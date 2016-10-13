#pragma once

#include <vector>
#include <string>
#include <memory>

#include "PhonemNodeFactory.hpp"
#include "phonems.hpp"
#include "PrefixTreeCompressor.hpp"


class PrefixTree
{
public:
	using t_phonem_ids = std::vector<std::size_t>;
	using t_word = std::vector<std::string>;

	enum : int
	{
		ERROR					= -1
		, WORD_NOT_FOUND		= -2
		, PHONEM_NOT_FOUND		= -3
		, ID_ELEMENT_NOT_FOUND	= -4
		, ELEMENT_WITH_PHONEM_NOT_FOUND	= -5
	};

public:
	PrefixTree(const t_phonems &i_phonems) 
		: phonems(i_phonems)
		, startElement(PhonemNodeFactory::create_start_node())
	{}

	/*
		add_word :
			word should be presented as vector of phonems
			word will be added to the Prefix Tree with ending by new node 'PhoneWord' with wordID
	*/
	bool add_word(const t_word & word, int wordId);

	/*
		get_word_id :
			returns the id of word in dictrionary or a ERROR_CODE
	*/
	int get_word_id(const t_word & word);

	/*
		Print the dictionary
	*/
	std::vector<std::string> get_all_words();

	/*
		Comress the tree with external compressor
	*/
	void compress(PrefixTreeCompressor * compressor) { compressor->compress(startElement.get()); };

private:

	bool add_phonems_ids_from_word(const t_word & word, t_phonem_ids & wordInPhonemsId);

	// find last elemnt in tree is mapped on the word 
	PhonemNode * find_last(PhonemNode * elemPtr, const t_phonem_ids & wordInPhonemsId, PhonemNode * &nodeWithID);

	void push_to_prefix_tree(const t_phonem_ids &wordInPhonemsId, int wordId);

	// compare if the element is 'Phonem' class and returns false otherwise
	bool is_phonems_equal(std::size_t phonem, PhonemNode * elem);

	// finding the phonem in neighbor nodes and returns the id or 'ELEMENT_WITH_PHONEM_NOT_FOUND'
	int find_phonem_in_tree(std::size_t phonemId, PhonemNode * elemPtr);

	// return pointer to the last inserted node
	PhonemNode * insert_in_tree(PhonemNode * elemPtr
								, t_phonem_ids::const_iterator itb
								, t_phonem_ids::const_iterator ite
								, int wordId);

	// get all words from dictionary started from 'node'
	void get_all_words_dfs(PhonemNode * node, std::string str, std::vector<std::string> & v_str, std::string wordId);

private:
	
	const t_phonems & phonems;

	std::unique_ptr<PhonemNode> startElement;


};