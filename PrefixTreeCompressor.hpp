#pragma once

#include <vector>
#include <map>
#include <set>
#include <functional>

#include "PhonemNode.hpp"

#include "DictionaryException.hpp"


/*
	PrefixTreeCompressor :
		- pure virtual class
		- used to compress a prefix tree
*/
class PrefixTreeCompressor
{
public:

	virtual void compress(PhonemNode * startNode) = 0;

	virtual ~PrefixTreeCompressor() {}
};



/*
	PrefixTreeCompressorFromEnd :
		Split compress process to:
			- move 'PhonemWord' nodes as close as possible to the Start node
			- start from final(have not any next pointers) 'Phonem' nodes 
			  and collapse the same nodes in a one common node
				-(1) make a container with previous collapsed nodes pointer
				-(2) while the container have more than 1 element
				-(3) if than nodes have common phonem collapse it
				-(4) goto 1
				
*/

class ComparatorPhonems
{
public:
	bool operator() (const PhonemNode * p1
		, const PhonemNode * p2)
	{
		if (p1->get_type() != p2->get_type())
		{
			if (p1->get_type() == PhonemNode::NODE_TYPE_PHONEM)
				return true;

			return false;
		}

		switch (p1->get_type())
		{
		case PhonemNode::NODE_TYPE_PHONEM:
			return PhonemNodeCast::to_phonem(p1)->get_phonem() < PhonemNodeCast::to_phonem(p2)->get_phonem();

		case PhonemNode::NODE_TYPE_WORD:
			return PhonemNodeCast::to_word(p1)->get_val() < PhonemNodeCast::to_word(p2)->get_val();

		case PhonemNode::NODE_TYPE_BEGIN:
			throw DictionaryException("Bad compare with Phonem Begin");
		}

		return false;
	}

};


class PrefixTreeCompressorFromEnd : public PrefixTreeCompressor
{
public:
	
	using t_nodes = std::vector<PhonemNode *>;
	//using t_last_nodes = std::multimap<std::size_t, std::pair<PhonemNode*, PhonemNode*>, decltype(cmp_phonem_nodes)>;
	using t_last_nodes = std::multimap<PhonemNode*, PhonemNode*, ComparatorPhonems>;
	using t_prev_collapsed_nodes = std::set<PhonemNode*>;

	/*virtual*/ 
	void compress(PhonemNode * startNode) override;

private:

	// move 'PhoneWord' elements as close as possible to PhoneStart element
	void move_word_elems(t_nodes stack, PhonemNode * nextElem);

	std::size_t find_index_to_node(PhonemNode * prevNode, PhonemNode * curNode);

	// get phonems without any next link (final node with type 'Phone')
	void get_last_phonems(t_nodes stack
							, PhonemNode * nextElem
							, t_last_nodes & mmapLastPhonems);

	// collapse nodes with same phonem ('mmapLastPhonems' wiil be empty)
	void collapse_phonems(t_last_nodes & mmapLastPhonems
						, t_prev_collapsed_nodes & nextMinimization);

	// using DFS to find previous node for all nodes from 'nextMinimization'
	void find_prev_nodes_for(t_nodes stack
							, PhonemNode * nextElem
							, const t_prev_collapsed_nodes & nextMinimization
							, t_last_nodes & mmapLastPhonems);

	// helper function to insert node to t_last_nodes
	void put_to_last_phonems(t_last_nodes & mmapLastPhonems, PhonemNode * prevNode, PhonemNode * lastNode)
	{
		mmapLastPhonems.insert(std::make_pair(lastNode, prevNode));
	};
};