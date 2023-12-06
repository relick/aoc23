module;

#include <array>
#include <format>
#include <string>
#include <vector>

export module Solutions.Day6;

import Utils.Parser;
import Utils.Input;

struct Race
{
	long long time;
	long long distance;
};

export std::string Day6SolveA( Utils::Input i )
{
	Utils::LineByLineParser parser;
	parser.AddToken( R"(Time:)" );
	parser.AddToken( R"(Distance:)" );
	parser.AddToken( R"(\d+)" );

	long long result = 1;

	std::vector<Race> races;

	for ( auto const& line : parser( i ) )
	{
		bool time = true;
		size_t raceNum = 0;
		for ( auto const& token : line )
		{
			switch ( token.matchIndex )
			{
			case 0: time = true; break;
			case 1: time = false; break;
			case 2:
			{
				if ( raceNum >= races.size() )
				{
					races.emplace_back();
				}
				if ( time )
				{
					races[ raceNum ].time = token.As<long long>();
				}
				else
				{
					races[ raceNum ].distance = token.As<long long>();
				}
				raceNum++;
				break;
			}
			}
		}
	}

	for ( Race const& race : races )
	{
		long long ways = 0;
		for ( long long t = 0; t < race.time; ++t )
		{
			if ( t * ( race.time - t ) > race.distance )
			{
				ways++;
			}
		}
		result *= ways;
	}

	return std::format( "{}", result );
}

export std::string Day6SolveB( Utils::Input i )
{
	Utils::LineByLineParser parser;
	parser.AddToken( R"(Time:)" );
	parser.AddToken( R"(Distance:)" );
	parser.AddToken( R"(\d+)" );

	Race race;

	for ( auto const& line : parser( i ) )
	{
		bool time = true;
		std::string n;
		for ( auto const& token : line )
		{
			switch ( token.matchIndex )
			{
			case 0: time = true; break;
			case 1: time = false; break;
			case 2:
			{
				n += token.token[ 0 ];
				break;
			}
			}
		}

		if ( time )
		{
			race.time = std::atoll( n.c_str() );
		}
		else
		{
			race.distance = std::atoll( n.c_str() );
		}
	}

	long long ways = 0;
	for ( long long t = 0; t < race.time; ++t )
	{
		if ( t * ( race.time - t ) > race.distance )
		{
			ways++;
		}
	}

	return std::format( "{}", ways );
}