#pragma once

#include <string>

#include "PrefixTree.hpp"


const std::size_t MAX_LINE_LENGTH = 256;

class Dictionary
{
public:
	// read from file and compress result trie
	Dictionary(std::string dictionaryfileName);

	// get word id
	int get_word_id(std::string str);
	
	void save_to_file(std::string fileName);

private:
	PrefixTree trie{ CONST_PHONEMS };
};






