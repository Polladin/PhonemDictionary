
#include <algorithm>

#include "PrefixTree.hpp"

#include "phonems.hpp"

bool PrefixTree::add_word(const t_word & word, int wordId)
{
	t_phonem_ids wordInPhonemsId;

	// spit word to phonems
	if (!add_phonems_ids_from_word(word, wordInPhonemsId))
		return false;

	push_to_prefix_tree(wordInPhonemsId, wordId);

	return true;
}


int PrefixTree::get_word_id(const t_word & word)
{
	t_phonem_ids wordInPhonemsId;

	// spit word to phonems
	if (!add_phonems_ids_from_word(word, wordInPhonemsId))
		return PHONEM_NOT_FOUND;
	
	// get last mapped node element in tree or 'nullptr'
	PhonemNode * nodeWithID{ nullptr };
	PhonemNode * elemPtr = find_last(startElement.get(), wordInPhonemsId, nodeWithID);

	if (elemPtr == nullptr)
		return WORD_NOT_FOUND;

	if (nodeWithID == nullptr)
	{
		for (const auto & elem : elemPtr->next)
			if (elem->get_type() == PhonemNode::NODE_TYPE_WORD)
				return PhonemNodeCast::to_word(elem.get())->get_val();
	}
	else
	{
		return PhonemNodeCast::to_word(nodeWithID)->get_val();
	}

	return ID_ELEMENT_NOT_FOUND;
}


PhonemNode * PrefixTree::find_last(PhonemNode * elemPtr, const t_phonem_ids & wordInPhonemsId, PhonemNode * &nodeWithID)
{
	for (const auto & ph : wordInPhonemsId)
	{
		int nextElem;
		if ((nextElem = find_phonem_in_tree(ph, elemPtr)) == ELEMENT_WITH_PHONEM_NOT_FOUND)
			return nullptr;

		elemPtr = elemPtr->next[nextElem].get();
		if (elemPtr->get_type() == PhonemNode::NODE_TYPE_WORD)
		{
			nodeWithID = elemPtr;
			elemPtr = elemPtr->next[0].get();
		}
	}

	return elemPtr;
}


bool PrefixTree::add_phonems_ids_from_word(const t_word & word, t_phonem_ids & wordInPhonemsId)
{
	// spit word to phonems
	for (const auto & ph : word)
	{
		// find the phonem in list of available phonems
		auto phonemElem = std::find(phonems.begin(), phonems.end(), ph);

		if (phonemElem == phonems.end())
		{
			// a phonem was not found;
			return false;
		}

		// add phonem as phonem id
		wordInPhonemsId.push_back(std::distance(phonems.begin(), phonemElem));
	}

	return true;
}


void PrefixTree::get_all_words_dfs(PhonemNode * node, std::string str, std::vector<std::string> & v_str, std::string wordId)
{
	if (node->get_type() == PhonemNode::NODE_TYPE_PHONEM)
		str += std::string(phonems[PhonemNodeCast::to_phonem(node)->get_phonem()]) + " ";
	else if (node->get_type() == PhonemNode::NODE_TYPE_WORD)
		wordId = "#" + std::to_string(PhonemNodeCast::to_word(node)->get_val()) + " ";

	if (node->next.size() == 0)
	{
		v_str.push_back(str+wordId);
		return;
	}

	for (std::size_t i = 0; i < node->next.size(); ++i)
		get_all_words_dfs(node->next[i].get(), str, v_str, wordId);
}


std::vector<std::string> PrefixTree::get_all_words()
{
	std::vector<std::string> v_str;
	PhonemNode * node = startElement.get();

	get_all_words_dfs(node, "", v_str, "");

	return v_str;
}


void PrefixTree::push_to_prefix_tree(const t_phonem_ids & wordInPhonemsId, int wordId)
{
	PhonemNode * elemPtr = startElement.get(); // get node PhonemBegin

	// find common path in the prefix tree
	for (std::size_t i = 0; i < wordInPhonemsId.size(); ++i)
	{
		// Is a node with the phonem reachable from current 'elemPtr' node
		int nextElem = find_phonem_in_tree(wordInPhonemsId[i], elemPtr);

		if (nextElem != ELEMENT_WITH_PHONEM_NOT_FOUND)
		{ // neighbor node with the same phonem is exist
			elemPtr = elemPtr->next[nextElem].get();
		}
		else
		{ // insert rest of phonems in prefix tree as new nodes and return the ptr tolast inserted node
			elemPtr 
				= insert_in_tree(elemPtr, wordInPhonemsId.begin() + i, wordInPhonemsId.end(), wordId);
			break;
		}
	}

	// insert in the end the node PhonemWord with WordId
	elemPtr->next.push_back(PhonemNodeFactory::create_word(wordId));
}


PhonemNode * PrefixTree::insert_in_tree( PhonemNode * elemPtr
										, t_phonem_ids::const_iterator itb
										, t_phonem_ids::const_iterator ite
										, int wordId )
{
	while (itb != ite)
	{
		elemPtr->next.push_back(PhonemNodeFactory::create_phonem(*itb));
		elemPtr = elemPtr->next.back().get();
		// next phonem
		++itb;
	}

	return elemPtr;
}


int PrefixTree::find_phonem_in_tree(std::size_t phonemId, PhonemNode * elemPtr)
{
	for (std::size_t j = 0; j < elemPtr->next.size(); ++j)
		if (is_phonems_equal(phonemId, elemPtr->next[j].get()))
			return j;

	return ELEMENT_WITH_PHONEM_NOT_FOUND;
}


bool PrefixTree::is_phonems_equal(std::size_t phonem, PhonemNode * elem)
{
	if (elem->get_type() == PhonemNode::NODE_TYPE_PHONEM
		&& PhonemNodeCast::to_phonem(elem)->get_phonem() == phonem)
		return true;

	if (elem->get_type() == PhonemNode::NODE_TYPE_WORD
		&& elem->next.size() == 1
		&& is_phonems_equal(phonem, elem->next[0].get()))
		return true;

	return false;
}
