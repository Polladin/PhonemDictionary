#pragma once

#include <string>

class DictionaryException
{
public:
	DictionaryException(std::string i_str)
		:str(i_str)
	{}

	std::string what() const { return str; }
	virtual ~DictionaryException() {}

private:
	std::string str;
};