module;

#include <format>
#include <string>

export module Solutions.Day1;

import Utils.Parser;
import Utils.Input;

export std::string Day1SolveA( Utils::Input i )
{
	Utils::LineByLineParser parser;
	parser.AddToken( R"(\d)" );

	int result = 0;

	for ( auto const& line : parser( i ) )
	{
		int const first = line.front().As<int>();
		int const last = line.back().As<int>();
		
		result += first * 10 + last;
	}

	return std::format( "{}", result );
}

export std::string Day1SolveB( Utils::Input i )
{
	Utils::LineByLineParser parser;
	parser.AddToken( R"(\d)", Utils::TokenType::Exclusive );
	parser.AddToken( R"(one)", Utils::TokenType::NonExclusive );
	parser.AddToken( R"(two)", Utils::TokenType::NonExclusive );
	parser.AddToken( R"(three)", Utils::TokenType::NonExclusive );
	parser.AddToken( R"(four)", Utils::TokenType::NonExclusive );
	parser.AddToken( R"(five)", Utils::TokenType::NonExclusive );
	parser.AddToken( R"(six)", Utils::TokenType::NonExclusive );
	parser.AddToken( R"(seven)", Utils::TokenType::NonExclusive );
	parser.AddToken( R"(eight)", Utils::TokenType::NonExclusive );
	parser.AddToken( R"(nine)", Utils::TokenType::NonExclusive );

	int result = 0;

	for ( auto const& line : parser( i ) )
	{
		auto const& firstTok = line.front();
		auto const& lastTok = line.back();
		int const first = firstTok.matchIndex > 0 ? firstTok.matchIndex : firstTok.As<int>();
		int const last = lastTok.matchIndex > 0 ? lastTok.matchIndex : lastTok.As<int>();

		result += first * 10 + last;
	}

	return std::format( "{}", result );
}