module;

#include <array>
#include <format>
#include <string>

export module Solutions.Day2;

import Utils.Parser;
import Utils.Input;

export std::string Day2SolveA( Utils::Input i )
{
	Utils::LineByLineParser parser;
	parser.AddToken( R"((\d+) red)" );
	parser.AddToken( R"((\d+) green)" );
	parser.AddToken( R"((\d+) blue)" );
	parser.AddToken( R"(Game (\d+))" );
	parser.AddToken( R"(;)" );

	int result = 0;

	for ( auto const& line : parser( i ) )
	{
		bool skip = false;

		int lineID = -1;
		std::array<int, 3> curCols{};

		auto valid = [ & ]()
			{
				return curCols[0] <= 12
					&& curCols[1] <= 13
					&& curCols[2] <= 14;
			};

		for ( auto const& token : line )
		{
			switch ( token.matchIndex )
			{
			case 0:
			case 1:
			case 2:
				curCols[ token.matchIndex ] = token.As<int>(); break;
			case 3: lineID = token.As<int>(); break;
			case 4: if ( !valid() ) { skip = true; break; }
			}

			if ( skip )
			{
				break;
			}
		}

		if ( !skip && valid() )
		{
			result += lineID;
		}
	}

	return std::format( "{}", result );
}

export std::string Day2SolveB( Utils::Input i )
{
	Utils::LineByLineParser parser;
	parser.AddToken( R"((\d+) red)" );
	parser.AddToken( R"((\d+) green)" );
	parser.AddToken( R"((\d+) blue)" );

	int result = 0;

	for ( auto const& line : parser( i ) )
	{
		std::array<int, 3> curCols{};

		for ( auto const& token : line )
		{
			curCols[ token.matchIndex ] = std::max( curCols[ token.matchIndex ], token.As<int>() );
		}

		result += curCols[ 0 ] * curCols[ 1 ] * curCols[ 2 ];
	}

	return std::format( "{}", result );
}