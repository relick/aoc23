module;

#include <array>
#include <format>
#include <ranges>
#include <string>

export module Solutions.Day3;

import Utils.Parser;
import Utils.Input;

export std::string Day3SolveA( Utils::Input i )
{
	Utils::GraphParser parser;
	auto partNodeType = parser.AddToken( R"(\d+)" );
	auto symbolNodeType = parser.AddToken( R"([^.\d])" );
	parser.Connect( partNodeType, symbolNodeType );

	int result = 0;

	for ( auto const& node : parser( i ) )
	{
		if ( node.token.matchIndex == 0 && node.connections.size() > 0)
		{
			result += node.token.As<int>();
		}
	}

	return std::format( "{}", result );
}

export std::string Day3SolveB( Utils::Input i )
{
	Utils::GraphParser parser;
	auto partNodeType = parser.AddToken( R"(\d+)" );
	auto gearNodeType = parser.AddToken( R"([*])" );
	parser.Connect( partNodeType, gearNodeType );

	int result = 0;

	for ( auto const& node : parser( i ) )
	{
		if ( node.token.matchIndex == 1 && node.connections.size() == 2 )
		{
			result += node.connections[ 0 ]->token.As<int>() * node.connections[ 1 ]->token.As<int>();
		}
	}

	return std::format( "{}", result );
}