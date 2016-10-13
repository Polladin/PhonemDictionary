
#include <fstream>
#include <sstream>

#include "Dictionary.hpp"

#include "DictionaryStringParser.hpp"
#include "DictionaryException.hpp"



Dictionary::Dictionary(std::string dictionaryfileName)
{

	std::ifstream inf(dictionaryfileName);

	if (!inf.is_open())
	{
		throw DictionaryException("Input file '" + dictionaryfileName + "' has been not opened");
	}

	char buf[MAX_LINE_LENGTH];

	while (inf.getline(buf, sizeof(buf)))
	{
		std::string str(buf);

		// if wrong format
		if (!DictionaryStringParser::is_correct_string(str))
			continue;

		// extract phonems and wordId
		auto ph		= DictionaryStringParser::get_phonems_from_input_string(str);
		auto wordId = DictionaryStringParser::get_word_id(str);
		

		// push word to trie
		trie.add_word(ph, wordId);
	}

	inf.close();
	
	// compress the trie
	std::unique_ptr<PrefixTreeCompressor> compressor(new PrefixTreeCompressorFromEnd());
	trie.compress(compressor.get());
}


int Dictionary::get_word_id(std::string str)
{
	auto ph = DictionaryStringParser::get_phonems(str);
	return trie.get_word_id(ph);
}


void Dictionary::save_to_file(std::string fileName)
{
	auto all_words = trie.get_all_words();

	std::ofstream outTreef(fileName);

	if (!outTreef.is_open())
	{
		throw DictionaryException("Output file '" + fileName + "' has been not opened/created");
	}

	for (const auto & e : all_words)
	{
		outTreef << e << "\n";
	}

	outTreef.close();
}


