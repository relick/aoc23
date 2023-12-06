module;

#include <fstream>
#include <string>
#include <vector>

export module Utils.Input;

export namespace Utils
{

class Input
{
	using Lines = std::vector<std::string>;
	Lines lines;

public:
	Input( std::string i_filename )
	{
		std::ifstream file( i_filename );
		std::string line;
		while ( std::getline( file, line ) )
		{
			lines.push_back( line );
		}
	}

	Lines::const_iterator begin() const { return lines.begin(); }
	Lines::const_iterator end() const { return lines.end(); }
};

}