#include <iostream>

#include "Dictionary.hpp"
#include "DictionaryException.hpp"
#include "PhonemNode.hpp"

PrefixTree tree(CONST_PHONEMS);

class wrapStr
{
public:

	wrapStr(int i_num, std::string i_str)
		:num{ i_num }, str{i_str}
	{}

	int num;
	std::string str;

	friend bool operator < (const wrapStr& lobj, const wrapStr& robj)
	{
		if (lobj.num < robj.num) return true;
		if (lobj.num > robj.num) return false;
		if (lobj.str < robj.str) return true;

		return false;
	}
};


Dictionary * dict{nullptr};

int main()
{
	try 
	{
		// create divtionary and minimize trie
		dict = new Dictionary("dictionary.txt");

		// find the word
		std::string word = "b r' i1 l' i1 a0 n t a4";

		auto wordId = dict->get_word_id(word);
		
		std::cout << "Word to find : " << word << "\n";
		if (wordId < 0)
			std::cout << "\t the word was not found : " << wordId << std::endl;
		else
			std::cout << "\t id : " << wordId << std::endl;


		// print count of nodes
		std::cout << "Count of nodes : " << PhonemNode::countOfNodes << std::endl;

		// save dictionary
		dict->save_to_file("dict.txt");
	}
	catch (const DictionaryException& ex)
	{
		std::cout << "Catch exception : " << ex.what() << std::endl;
	}

	std::cin.get();
}// main
