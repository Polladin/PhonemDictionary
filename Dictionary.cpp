
#include <fstream>
#include <sstream>
//#include <map>

#include "Dictionary.hpp"

#include "DictionaryStringParser.hpp"
#include "DictionaryException.hpp"



class StrCmp
{
public:
	bool operator() (const std::pair<int, std::string> & e1
						, const std::pair<int, std::string> & e2)
	{
		if (e1.first < e2.first) return true;
		if (e1.first > e2.first) return false;

		return e1.second < e2.second;
	}
};


Dictionary::Dictionary(std::string dictionaryfileName)
{

	std::ifstream inf(dictionaryfileName);

	if (!inf.is_open())
	{
		throw DictionaryException("Input file '" + dictionaryfileName + "' has been not opened");
	}

	char buf[MAX_LINE_LENGTH];


	//std::multiset<std::pair<int, std::string>, StrCmp> mmap;

	while (inf.getline(buf, sizeof(buf)))
	{
		std::string str(buf);

		// if wrong format
		if (!DictionaryStringParser::is_correct_string(str))
			continue;

		// extract phonems and wordId
		auto ph		= DictionaryStringParser::get_phonems_from_input_string(str);
		auto wordId = DictionaryStringParser::get_word_id(str);
		
		//std::string tmpstr;
		//for (const auto & e : ph)
		//{
		//	tmpstr += e + " ";
		//}
		//tmpstr += "#" + std::to_string(wordId);

		//mmap.insert(std::make_pair(wordId, tmpstr));

		// push word to trie
		trie.add_word(ph, wordId);
	}

	
	//std::ofstream outTreef("test");
	//for (const auto & e : mmap)
	//{
	//	outTreef << e.second << "\n";
	//}
	//outTreef.close();


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

	//std::multiset<std::pair<int, std::string>, StrCmp> mmap;

	//for (const auto & e : all_words)
	//{
	//	mmap.insert(std::make_pair(std::stoi(e.substr(e.find('#')+1)),e));
	//}

	std::ofstream outTreef(fileName);

	if (!outTreef.is_open())
	{
		throw DictionaryException("Output file '" + fileName + "' has been not opened/created");
	}

	for (const auto & e : all_words)  //mmap
	{
		outTreef << e << "\n";
	}

	outTreef.close();
}


