module;

#include <array>
#include <format>
#include <list>
#include <map>
#include <string>

export module Solutions.Day8;

import Utils.Parser;
import Utils.Input;

struct Node
{
	std::string key;
	Node* left{ nullptr };
	Node* right{ nullptr };
};

enum class Dir
{
	Left,
	Right,
};

export std::string Day8SolveA( Utils::Input i )
{
	Utils::LineByLineParser parser;
	parser.AddToken( R"((\w{3}) = \((\w{3}), (\w{3})\))" );
	parser.AddToken( R"(R|L)" );

	std::map<std::string, Node*> cache;
	std::list<Node> nodes;

	auto GetNode = [ &cache, &nodes ]( std::string const& key ) -> Node&
		{
			auto cacheI = cache.find( key );
			if ( cacheI != cache.end() )
			{
				return *( cacheI->second );
			}
			else
			{
				Node& node = nodes.emplace_back();
				cache.emplace( key, &node );
				node.key = key;
				return node;
			}
		};

	std::vector<Dir> directions;

	for ( auto const& line : parser( i ) )
	{
		for ( auto const& token : line )
		{
			switch ( token.matchIndex )
			{
			case 0:
			{
				Node& node = GetNode( token.token[ 0 ] );
				node.left = &GetNode( token.token[ 1 ] );
				node.right = &GetNode( token.token[ 2 ] );
				break;
			}
			case 1:
			{
				directions.push_back( token.token[ 0 ][ 0 ] == 'R' ? Dir::Right : Dir::Left );
				break;
			}
			}
		}
	}

	Node* cur = cache.find( "AAA" )->second;

	int steps = 0;
	while ( cur->key != "ZZZ" )
	{
		for ( Dir dir : directions )
		{
			cur = dir == Dir::Left ? cur->left : cur->right;
			++steps;
			if ( cur->key == "ZZZ" )
			{
				break;
			}
		}
	}

	return std::format( "{}", steps );
}

export std::string Day8SolveB( Utils::Input i )
{
	Utils::LineByLineParser parser;
	parser.AddToken( R"((\w{3}) = \((\w{3}), (\w{3})\))" );
	parser.AddToken( R"(R|L)" );

	std::map<std::string, Node*> cache;
	std::map<std::string, Node*> endInA;
	std::list<Node> nodes;

	auto GetNode = [ &cache, &nodes ]( std::string const& key ) -> Node&
		{
			auto cacheI = cache.find( key );
			if ( cacheI != cache.end() )
			{
				return *( cacheI->second );
			}
			else
			{
				Node& node = nodes.emplace_back();
				node.key = key;
				cache.emplace( key, &node );
				if ( key[ 2 ] == 'A' )
				{
					cache.emplace( key, &node );
				}
				return node;
			}
		};

	std::vector<Dir> directions;

	for ( auto const& line : parser( i ) )
	{
		for ( auto const& token : line )
		{
			switch ( token.matchIndex )
			{
			case 0:
			{
				Node& node = GetNode( token.token[ 0 ] );
				node.left = &GetNode( token.token[ 1 ] );
				node.right = &GetNode( token.token[ 2 ] );
				break;
			}
			case 1:
			{
				directions.push_back( token.token[ 0 ][ 0 ] == 'R' ? Dir::Right : Dir::Left );
				break;
			}
			}
		}
	}

	Node* cur = cache.find( "AAA" )->second;

	int steps = 0;
	while ( cur->key != "ZZZ" )
	{
		for ( Dir dir : directions )
		{
			cur = dir == Dir::Left ? cur->left : cur->right;
			++steps;
			if ( cur->key == "ZZZ" )
			{
				break;
			}
		}
	}

	return std::format( "{}", steps );
}