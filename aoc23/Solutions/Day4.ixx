module;

#include <format>
#include <string>
#include <unordered_map>
#include <unordered_set>

export module Solutions.Day4;

import Utils.Parser;
import Utils.Input;

export std::string Day4SolveA( Utils::Input i )
{
	Utils::LineByLineParser parser;
	parser.AddToken( R"(Card\s+\d+:)" );
	parser.AddToken( R"(\d+)" );
	parser.AddToken( R"([|])" );

	int result = 0;

	for ( auto const& line : parser( i ) )
	{
		bool winningNums = true;
		std::unordered_set<int> nums;
		int numWins = 0;
		for ( auto const& token : line )
		{
			switch ( token.matchIndex )
			{
			case 0: break;
			case 1:
			{
				if ( winningNums )
				{
					nums.insert( token.As<int>() );
				}
				else
				{
					if ( nums.contains( token.As<int>() ) )
					{
						numWins++;
					}
				}
			}
			break;
			case 2: winningNums = false; break;
			}
		}

		if ( numWins > 0 )
		{
			int val = 1;
			for ( int i = 1; i < numWins; ++i )
			{
				val *= 2;
			}
			result += val;
		}
	}

	return std::format( "{}", result );
}

export std::string Day4SolveB( Utils::Input i )
{
	Utils::LineByLineParser parser;
	parser.AddToken( R"(Card\s+(\d+):)" );
	parser.AddToken( R"(\d+)" );
	parser.AddToken( R"([|])" );

	int result = 0;

	std::unordered_map<int, int> copies;

	for ( auto const& line : parser( i ) )
	{
		bool winningNums = true;
		std::unordered_set<int> nums;
		int numWins = 0;
		int cardID = 0;
		for ( auto const& token : line )
		{
			switch ( token.matchIndex )
			{
			case 0: cardID = token.As<int>(); break;
			case 1:
			{
				if ( winningNums )
				{
					nums.insert( token.As<int>() );
				}
				else
				{
					if ( nums.contains( token.As<int>() ) )
					{
						numWins++;
					}
				}
			}
			break;
			case 2: winningNums = false; break;
			}
		}

		copies[ cardID ] += 1;
		int count = copies[ cardID ];
		for ( int i = 0; i < numWins; ++i )
		{
			copies[ cardID + i + 1 ] += count;
		}
	}

	for ( auto const& copy : copies )
	{
		result += copy.second;
	}

	return std::format( "{}", result );
}