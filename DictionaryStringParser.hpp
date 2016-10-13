#pragma once

#include <vector>


/*
DictionaryStringParser :
Parse Strings from input file
*/
class DictionaryStringParser
{
public:

	static bool is_correct_string(const std::string & str);

	static std::vector<std::string> get_phonems_from_input_string(const std::string str);

	static int get_word_id(const std::string & str);

	static  std::vector<std::string> get_phonems(const std::string str);

private:

	static std::string get_str_second_part(const std::string & str);

	static std::string get_phonems_str(const std::string & str);
};