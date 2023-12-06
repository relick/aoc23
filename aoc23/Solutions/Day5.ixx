module;

#include <algorithm>
#include <array>
#include <format>
#include <optional>
#include <ranges>
#include <string>
#include <vector>

export module Solutions.Day5;

import Utils.Parser;
import Utils.Input;

struct Conversion
{
	long long start;
	long long end;
	long long newStart;

	std::optional<long long> operator()( long long i_source ) const
	{
		if ( i_source >= start && i_source < end )
		{
			return newStart + i_source - start;
		}
		return std::nullopt;
	}
};

struct Layer
{
	std::vector<Conversion> conversions;
	long long Convert( long long i_source ) const
	{
		for ( Conversion const& conversion : conversions )
		{
			std::optional<long long> converted = conversion( i_source );
			if ( converted )
			{
				return *converted;
			}
		}

		return i_source;
	}

	std::vector<std::pair<long long, long long>> ConvertRange( std::pair<long long, long long> i_source ) const
	{
		std::vector<std::pair<long long, long long>> newRanges;
		while ( i_source.first < i_source.second )
		{
			for ( int i = 0; i < conversions.size();)
			{
				Conversion const& con = conversions[ i ];
				std::optional<long long> converted = con( i_source.first );
				if ( converted )
				{
					if ( i_source.second < con.end )
					{
						newRanges.emplace_back( *converted, *con( i_source.second ) );
						i_source.first = i_source.second;
						break;
					}
					else
					{
						newRanges.emplace_back( *converted, *con( con.end - 1 ) );
						i_source.first = con.end;
					}
				}
				else
				{
					if ( i_source.second < con.start )
					{
						newRanges.emplace_back( i_source.first, i_source.second );
						i_source.first = i_source.second;
						break;
					}
					else if ( i_source.first >= con.end )
					{
						if ( i == conversions.size() - 1 )
						{
							newRanges.emplace_back( i_source.first, i_source.second );
							i_source.first = i_source.second;
							break;
						}
					}
					else
					{
						newRanges.emplace_back( i_source.first, con.start );
						i_source.first = con.start;
						continue;
					}
				}

				++i;
			}
		}

		return newRanges;
	}
};

export std::string Day5SolveA( Utils::Input i )
{
	Utils::LineByLineParser parser;
	parser.AddToken( R"(seeds:)" );
	parser.AddToken( R"( (\d+))" );
	parser.AddToken( R"(^(\d+) (\d+) (\d+)$)" );
	parser.AddToken( R"((?:seed-to-soil map:)|)"
		R"((?:soil-to-fertilizer map:)|)"
		R"((?:fertilizer-to-water map:)|)"
		R"((?:water-to-light map:)|)"
		R"((?:light-to-temperature map:)|)"
		R"((?:temperature-to-humidity map:)|)"
		R"((?:humidity-to-location map:))" );

	std::vector<long long> seeds;
	std::vector<Layer> layers;
	Layer* curLayer = nullptr;

	for ( auto const& line : parser( i ) )
	{
		for ( auto const& token : line )
		{
			switch ( token.matchIndex )
			{
			case 0: break;
			case 1: seeds.push_back( token.As<long long>() ); break;
			case 2:
			{
				Conversion& conversion = curLayer->conversions.emplace_back();
				conversion.newStart = token.As<long long>( 0 );
				conversion.start = token.As<long long>( 1 );
				conversion.end = conversion.start + token.As<long long>( 2 );
				break;
			}
			case 3:
			{
				curLayer = &layers.emplace_back();
				break;
			}
			}
		}
	}

	long long minLocation = LLONG_MAX;
	for ( long long seedToLoc : seeds )
	{
		for ( Layer const& layer : layers )
		{
			seedToLoc = layer.Convert( seedToLoc );
		}
		minLocation = std::min( minLocation, seedToLoc );
	}

	return std::format( "{}", minLocation );
}

export std::string Day5SolveB( Utils::Input i )
{
	Utils::LineByLineParser parser;
	parser.AddToken( R"(seeds:)" );
	parser.AddToken( R"( (\d+) (\d+))" );
	parser.AddToken( R"(^(\d+) (\d+) (\d+)$)" );
	parser.AddToken( R"((?:seed-to-soil map:)|)"
		R"((?:soil-to-fertilizer map:)|)"
		R"((?:fertilizer-to-water map:)|)"
		R"((?:water-to-light map:)|)"
		R"((?:light-to-temperature map:)|)"
		R"((?:temperature-to-humidity map:)|)"
		R"((?:humidity-to-location map:))" );

	std::vector<std::pair<long long, long long>> seedRanges;
	std::vector<Layer> layers;
	Layer* curLayer = nullptr;

	for ( auto const& line : parser( i ) )
	{
		for ( auto const& token : line )
		{
			switch ( token.matchIndex )
			{
			case 0: break;
			case 1:
			{
				long long start = token.As<long long>( 0 );
				seedRanges.emplace_back( start, start + token.As<long long>( 1 ) );
				break;
			}
			case 2:
			{
				Conversion& conversion = curLayer->conversions.emplace_back();
				conversion.newStart = token.As<long long>( 0 );
				conversion.start = token.As<long long>( 1 );
				conversion.end = conversion.start + token.As<long long>( 2 );
				break;
			}
			case 3:
			{
				curLayer = &layers.emplace_back();
				break;
			}
			}
		}
	}

	for ( Layer& layer : layers )
	{
		std::sort( layer.conversions.begin(), layer.conversions.end(), []( Conversion const& a, Conversion const& b ) { return a.start < b.start; } );
	}

	long long minLocation = LLONG_MAX;
	for ( std::pair<long long, long long> seedRange : seedRanges )
	{
		std::vector<std::pair<long long, long long>> curRanges{ seedRange };
		std::vector<std::pair<long long, long long>> newRanges;
		for ( Layer const& layer : layers )
		{
			for ( std::pair<long long, long long> range : curRanges )
			{
				newRanges.insert_range( newRanges.end(), layer.ConvertRange( range ) );
			}
			std::swap( curRanges, newRanges );
			newRanges.clear();
		}

		for ( std::pair<long long, long long> range : curRanges )
		{
			minLocation = std::min( minLocation, range.first );
		}

		/*for ( long long seed = seedRange.first; seed < seedRange.second; ++seed )
		{
			long long seedToLoc = seed;
			for ( Layer const& layer : layers )
			{
				seedToLoc = layer.Convert( seedToLoc );
			}
			minLocation = std::min( minLocation, seedToLoc );
		}*/
	}

	return std::format( "{}", minLocation );
}