#include <word_reader.h>

misc::WordReader::WordReader(std::string fname, CharConversion cc) : mFile(fname), mConversion(cc)
{
	if (!mFile.is_open())
		throw std::ios::failure(std::string("error opening file '") + fname + "'");
}


misc::WordReader::~WordReader()
{
	mFile.close();
}


bool misc::WordReader::done()
{
	return !mFile.is_open() || mFile.eof();
}


std::string misc::WordReader::get_next()
{
	//std::string temp;// = "?";
	//std::getline(mFile, temp);
	char c;
	while (!mFile.eof())
	{
		mFile.get(c);
		if (c < 0)
		{
			// A garbage character...not sure why these are here?
			continue;
		}
		if (mFile.eof())
		{
			// That was the last character -- return the partial word as-is
			return mPartialWord;
		}
		else if (isalpha(c))
		{
			if (mConversion == CharConversion::LOWER && c >= 'A' && c <= 'Z')
				c = (c - 'A') + 'a';
			else if (mConversion == CharConversion::UPPER && c >= 'a' && c <= 'z')
				c = (c - 'a') + 'A';
			//temp[0] = c;
			mPartialWord.append(1, c);
		}
		else if (ispunct(c) || isspace(c))
		{
			// We've reached the end of the word -- reset the partial word and then return it
			std::string return_val = mPartialWord;
			mPartialWord = "";
			if (return_val.length() > 0)
				return return_val;
		}
	}

	return "";
}