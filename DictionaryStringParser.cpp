
#include <sstream>

#include "DictionaryStringParser.hpp"


std::vector<std::string> DictionaryStringParser::get_phonems_from_input_string(const std::string str)
{
	return get_phonems(get_phonems_str(str));
}


std::vector<std::string> DictionaryStringParser::get_phonems(const std::string str)
{
	std::stringstream phonems(str);
	std::vector<std::string> ph;

	while (!phonems.eof())
	{
		std::string tmp_ph;
		phonems >> tmp_ph;
		ph.push_back(tmp_ph);
	}

	return ph;
}


bool DictionaryStringParser::is_correct_string(const std::string & str)
{
	if (str.find('=') == std::string::npos
		|| str.find('#') == std::string::npos)
		return false;

	return true;
}

std::string DictionaryStringParser::get_str_second_part(const std::string & str)
{
	return str.substr(str.find('=') + 1);
}

std::string DictionaryStringParser::get_phonems_str(const std::string & str)
{
	std::string phonemsAndId = get_str_second_part(str);
	return phonemsAndId.substr(0, phonemsAndId.find('#'));
}

int DictionaryStringParser::get_word_id(const std::string & str)
{
	return std::stoi(str.substr(str.find('#') + 1));
}
