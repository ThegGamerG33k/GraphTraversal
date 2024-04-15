#pragma once
#include <fstream>
#include <string>

namespace misc
{
	class WordReader
	{
	public:
		enum class CharConversion { NONE, UPPER, LOWER };
	protected:
		std::ifstream mFile;
		CharConversion mConversion;
		std::string mPartialWord;
	public:
		WordReader(std::string fname, CharConversion cc);
		~WordReader();
		bool done();
		std::string get_next();
	};
}
