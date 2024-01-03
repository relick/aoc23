#include <functional>
#include <iostream>
#include <optional>
#include <string>
#include <unordered_map>
#include <fstream>

import Solutions.Day1;
import Solutions.Day2;
import Solutions.Day3;
import Solutions.Day4;
import Solutions.Day5;
import Solutions.Day6;
import Solutions.Day7;
import Solutions.Day8;
import Utils.AoC;
import Utils.Input;

struct Solver
{
	std::function<std::string( Utils::Input )> solveA;
	std::function<std::string( Utils::Input )> solveB;
};

// comment out assignment to query user
std::optional<int> const s_dayOverride = 8;

int GetDay( char const* i_arg )
{
	if ( i_arg )
	{
		return std::atoi( i_arg );
	}
	if ( s_dayOverride )
	{
		return *s_dayOverride;
	}
	std::cout << "Day?" << std::endl;
	int day;
	std::cin >> day;
	return day;
}

// comment out assignment to query user
std::optional<AoC::Part> const s_partOverride = AoC::Part::B;

AoC::Part GetPart( char const* i_arg )
{
	char const* str = i_arg;
	std::string partStr;
	if ( !str )
	{
		if ( s_partOverride )
		{
			return *s_partOverride;
		}

		std::cout << "Part?" << std::endl;
		std::cin >> partStr;
		str = partStr.c_str();
	}
	return str[ 0 ] == 'A' || str[ 0 ] == 'a' ? AoC::Part::A : ( str[ 1 ] == 'O' || str[ 1 ] == 'o' ? AoC::Part::Both : AoC::Part::B );
}

int main(int i_argCount, char const** i_args)
{
	std::unordered_map<int, Solver> solvers{
		{1, {Day1SolveA, Day1SolveB}},
		{2, {Day2SolveA, Day2SolveB}},
		{3, {Day3SolveA, Day3SolveB}},
		{4, {Day4SolveA, Day4SolveB}},
		{5, {Day5SolveA, Day5SolveB}},
		{6, {Day6SolveA, Day6SolveB}},
		{7, {Day7SolveA, Day7SolveB}},
		{8, {Day8SolveA, Day8SolveB}},
	};

	int const day = GetDay( i_argCount > 1 ? i_args[ 1 ] : nullptr );
	AoC::Part const part = GetPart( i_argCount > 2 ? i_args[ 2 ] : nullptr );

	if ( part == AoC::Part::A || part == AoC::Part::Both )
	{
		std::cout << solvers.at( day ).solveA( Utils::Input( "Inputs/8.txt" ) ) << std::endl;
	}

	if ( part == AoC::Part::B || part == AoC::Part::Both )
	{
		std::cout << solvers.at( day ).solveB( Utils::Input( "Inputs/8test3.txt" ) ) << std::endl;
	}

	return 0;
}
