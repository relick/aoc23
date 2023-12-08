module;

#include <algorithm>
#include <array>
#include <format>
#include <numeric>
#include <map>
#include <ranges>
#include <string>
#include <vector>

export module Solutions.Day7;

import Utils.Parser;
import Utils.Input;

using Card = int;

enum class Rank
{
	HighCard,
	OnePair,
	TwoPair,
	ThreeOfAKind,
	FullHouse,
	FourOfAKind,
	FiveOfAKind,
};

struct Hand
{
	std::vector<Card> cards;
	int bet;

	Rank rank;

	void StoreRank( std::optional<Card> jokerValue )
	{
		int jokers = 0;
		std::map<Card, int> cardCounts;
		for ( Card c : cards )
		{
			if ( jokerValue && c == *jokerValue )
			{
				jokers++;
				continue;
			}
			cardCounts[ c ]++;
		}

		if ( cardCounts.size() <= 1 )
		{
			rank = Rank::FiveOfAKind;
		}
		else if ( cardCounts.size() == 2 )
		{
			rank = std::ranges::any_of( cardCounts, [jokers]( auto const& count ) { return count.second + jokers >= 4; } )
				? Rank::FourOfAKind
				: Rank::FullHouse;
		}
		else if ( cardCounts.size() == 3 )
		{
			rank = std::ranges::any_of( cardCounts, [jokers]( auto const& count ) { return count.second + jokers >= 3; } )
				? Rank::ThreeOfAKind
				: Rank::TwoPair;
		}
		else
		{
			rank = cardCounts.size() == 4 ? Rank::OnePair : Rank::HighCard;
		}

	}
};

bool operator<( Hand const& a, Hand const& b )
{
	if ( a.rank == b.rank )
	{
		for ( int i = 0; i < a.cards.size(); ++i )
		{
			if ( a.cards[ i ] != b.cards[ i ] )
			{
				return a.cards[ i ] < b.cards[ i ];
			}
		}
	}
	return a.rank < b.rank;
}

export std::string Day7SolveA( Utils::Input i )
{
	Utils::LineByLineParser parser;
	parser.AddToken( R"( (\d+))" );
	parser.AddToken( R"(2)" );
	parser.AddToken( R"(3)" );
	parser.AddToken( R"(4)" );
	parser.AddToken( R"(5)" );
	parser.AddToken( R"(6)" );
	parser.AddToken( R"(7)" );
	parser.AddToken( R"(8)" );
	parser.AddToken( R"(9)" );
	parser.AddToken( R"(T)" );
	parser.AddToken( R"(J)" );
	parser.AddToken( R"(Q)" );
	parser.AddToken( R"(K)" );
	parser.AddToken( R"(A)" );

	int result = 0;

	std::vector<Hand> hands;

	for ( auto const& line : parser( i ) )
	{
		Hand& curHand = hands.emplace_back();
		for ( auto const& token : line )
		{
			switch ( token.matchIndex )
			{
			case 0:
			{
				curHand.bet = token.As<int>();
				break;
			}
			default:
			{
				curHand.cards.push_back( token.matchIndex );
				break;
			}
			}
		}
		curHand.StoreRank( std::nullopt );
	}

	std::ranges::sort( hands, operator< );

	int rank = 1;
	for ( Hand const& hand : hands )
	{
		result += hand.bet * rank++;
	}

	return std::format( "{}", result );
}

export std::string Day7SolveB( Utils::Input i )
{
	Utils::LineByLineParser parser;
	parser.AddToken( R"( (\d+))" );
	parser.AddToken( R"(J)" );
	parser.AddToken( R"(2)" );
	parser.AddToken( R"(3)" );
	parser.AddToken( R"(4)" );
	parser.AddToken( R"(5)" );
	parser.AddToken( R"(6)" );
	parser.AddToken( R"(7)" );
	parser.AddToken( R"(8)" );
	parser.AddToken( R"(9)" );
	parser.AddToken( R"(T)" );
	parser.AddToken( R"(Q)" );
	parser.AddToken( R"(K)" );
	parser.AddToken( R"(A)" );

	int result = 0;

	std::vector<Hand> hands;

	for ( auto const& line : parser( i ) )
	{
		Hand& curHand = hands.emplace_back();
		for ( auto const& token : line )
		{
			switch ( token.matchIndex )
			{
			case 0:
			{
				curHand.bet = token.As<int>();
				break;
			}
			default:
			{
				curHand.cards.push_back( token.matchIndex );
				break;
			}
			}
		}
		curHand.StoreRank( 1 );
	}

	std::ranges::sort( hands, operator< );

	int rank = 1;
	for ( Hand const& hand : hands )
	{
		result += hand.bet * rank++;
	}

	return std::format( "{}", result );
}