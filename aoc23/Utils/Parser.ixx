module;

#include <charconv>
#include <concepts>
#include <list>
#include <map>
#include <regex>
#include <vector>
#include <unordered_map>

export module Utils.Parser;

import Utils.Input;

export namespace Utils
{

struct Token
{
	std::vector<std::string> token;
	std::string full;
	int matchIndex{ -1 };

	template<std::integral T_Int>
	T_Int As(size_t which = 0) const
	{
		T_Int t;
		std::from_chars( token[ which ].c_str(), token[ which ].c_str() + token[ which ].size(), t );
		return t;
	}
};

using Line = std::vector<Token>;
using Lines = std::vector<Line>;

enum class TokenType
{
	Exclusive,
	NonExclusive,
};

class LineByLineParser
{
	std::vector<std::pair<std::regex, TokenType>> tokenisers;

public:

	void AddToken( std::string i_regex, TokenType i_type = TokenType::Exclusive )
	{
		tokenisers.emplace_back( i_regex, i_type );
	}

	Lines operator()( Utils::Input const& i_input )
	{
		Lines lines;

		for ( std::string const& str : i_input )
		{
			Line& line = lines.emplace_back();

			std::string remaining( str );
			while ( !remaining.empty() )
			{
				bool matched = false;
				int matchIndex = 0;
				for ( auto const& regex : tokenisers )
				{
					std::smatch match;
					if ( std::regex_search( remaining, match, regex.first ) && match.prefix().length() == 0 )
					{
						Token& newTok = line.emplace_back();
						newTok.full = match[ 0 ].str();
						for ( size_t i = 1; i < match.size(); ++i )
						{
							newTok.token.push_back( match[ i ].str() );
						}
						if ( newTok.token.empty() )
						{
							newTok.token.push_back( newTok.full );
						}
						newTok.matchIndex = matchIndex;

						if ( regex.second == TokenType::Exclusive )
						{
							remaining = remaining.substr( newTok.full.size() );
							matched = true;
						}
						break;
					}
					++matchIndex;
				}

				if ( !matched )
				{
					remaining = remaining.substr( 1 );
				}
			}
		}

		return lines;
	}
};

struct Node
{
	Token token;
	std::vector<Node*> connections;
};

using CoordList = std::vector<std::pair<int, int>>;
CoordList Through( int line, int start, size_t size )
{
	CoordList list;
	for ( int x = start; x < start + ( int )size; ++x )
	{
		list.emplace_back( x, line );
	}
	return list;
}
CoordList Around( int line, int start, size_t size )
{
	CoordList list;
	for ( int y = line - 1; y <= line + 1; ++y )
	{
		list.emplace_back( start - 1, y );
		list.emplace_back( start + ( int )size, y );
	}
	for ( int x = start - 1; x <= start + ( int )size; ++x )
	{
		list.emplace_back( x, line - 1 );
		list.emplace_back( x, line + 1 );
	}
	return list;
}

using Graph = std::list<Node>;

class GraphParser
{
	std::vector<std::regex> tokenisers;
	std::unordered_multimap<int, int> connectionRules;

public:

	int AddToken( std::string i_regex )
	{
		tokenisers.emplace_back( i_regex );
		return static_cast<int>( tokenisers.size() - 1 );
	}

	void Connect( int i_item1, int i_item2 )
	{
		connectionRules.emplace( i_item1, i_item2 );
		connectionRules.emplace( i_item2, i_item1 );
	}

	Graph operator()( Utils::Input const& i_input )
	{
		Graph nodes;
		std::map<std::pair<int, int>, Node*> cache;

		int line = 0;
		for ( std::string const& str : i_input )
		{
			std::string remaining( str );
			int start = 0;
			while ( !remaining.empty() )
			{
				bool matched = false;
				int matchIndex = 0;
				for ( auto const& regex : tokenisers )
				{
					std::smatch match;
					if ( std::regex_search( remaining, match, regex ) && match.prefix().length() == 0 )
					{
						Node& newNode = nodes.emplace_back();
						newNode.token.full = match[ 0 ].str();
						for ( size_t i = 1; i < match.size(); ++i )
						{
							newNode.token.token.push_back( match[ i ].str() );
						}
						if ( newNode.token.token.empty() )
						{
							newNode.token.token.push_back( newNode.token.full );
						}
						newNode.token.matchIndex = matchIndex;

						remaining = remaining.substr( newNode.token.full.size() );
						matched = true;

						auto connections = connectionRules.equal_range( matchIndex );
						for ( auto iter = connections.first; iter != connections.second; ++iter )
						{
							int connection = iter->second;
							for ( std::pair<int, int> coord : Around( line, start, newNode.token.token.size() ) )
							{
								auto cacheI = cache.find( coord );
								if ( cacheI != cache.end() )
								{
									auto has = std::find( newNode.connections.begin(), newNode.connections.end(), cacheI->second ) != newNode.connections.end();
									if ( !has )
									{
										newNode.connections.push_back( cacheI->second );
										cacheI->second->connections.push_back( &newNode );
									}
								}
							}
						}

						for ( std::pair<int, int> coord : Through( line, start, newNode.token.token.size() ) )
						{
							cache[ coord ] = &newNode;
						}

						start += ( int )newNode.token.full.size();

						break;
					}
					++matchIndex;
				}

				if ( !matched )
				{
					remaining = remaining.substr( 1 );
					start += 1;
				}
			}

			++line;
		}

		return nodes;
	}
};

}