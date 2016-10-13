

#include "PrefixTreeCompressor.hpp"

#include "DictionaryException.hpp"


void PrefixTreeCompressorFromEnd::compress(PhonemNode * startNode)
{
	// move 'PhoneWord' elements as close as possible to PhoneStart element
	move_word_elems(t_nodes(), startNode);

	// get phonems without any next link (final node with type 'Phone')
	t_last_nodes mmapLastPhonems;

	get_last_phonems(t_nodes(), startNode, mmapLastPhonems);

	// container for next nodes possible can be minimized
	t_prev_collapsed_nodes nextMinimization;
	
	// collapse nodes with same phonem ('mmapLastPhonems' wiil be empty)
	collapse_phonems(mmapLastPhonems, nextMinimization);

	// while have nodes for next minimization
	while (nextMinimization.size() != 0)
	{
		// using DFS to find previous node for all nodes from 'nextMinimization'
		find_prev_nodes_for(t_nodes(), startNode, nextMinimization, mmapLastPhonems);

		// clear 'nextMinimization' for new a iteration 
		nextMinimization.clear();

		// collapse nodes with same phonem ('mmapLastPhonems' wiil be empty)
		collapse_phonems(mmapLastPhonems, nextMinimization);
	}
}



void PrefixTreeCompressorFromEnd::move_word_elems(t_nodes stack, PhonemNode * nextElem)
{

	if (nextElem->get_type() == PhonemNode::NODE_TYPE_WORD)
	{
		// minimal size of stack = 1 (0 element - NODE_TYPE_BEGIN)
		std::size_t idx = stack.size() - 1;

		// check how many nodes in the branch have only 1 pointer
		while (idx > 0 && stack[idx]->next.size() == 1)
			--idx;

		// if current node or previous node has more than 1 pointer
		// NODE_TYPE_WORD element is not able to move
		if (idx == stack.size() - 1 || idx + 1 == stack.size() - 1)
			return;

		// found left node has more than 1 child
		// need to know index of necessary link
		std::size_t idxInLeftNode = find_index_to_node(stack[idx], stack[idx+1]);

		PhonemNode * nodeLeft = stack[idx];
		PhonemNode * nodeRight = stack[stack.size() - 1];

		// swap pinters between left node and right node 
		//		LeftNode -> NODE_TYPE_WORD
		//		RightNode - > some nodes -> RightNode -> ...
		nodeLeft->next[idxInLeftNode].swap(nodeRight->next[0]);

		// insert pointer to 'PhonemWord' node
		//		LeftNode -> NODE_TYPE_WORD - > some nodes -> RightNode -> nullptr
		nextElem->next.emplace_back(std::move(nodeRight->next[0]));

		//		RightNode -> nullptr (!need to clear link container)
		nodeRight->next.clear();

		return;
	}

	// push NODE_TYPE_PHONEM to stack
	stack.push_back(nextElem);

	// recursive DFS
	for (std::size_t i = 0; i < stack.back()->next.size(); ++i)
	{
		move_word_elems(stack, stack.back()->next[i].get());
	}
}


std::size_t PrefixTreeCompressorFromEnd::find_index_to_node(PhonemNode * prevNode, PhonemNode * curNode)
{
	for (std::size_t i = 0; i < prevNode->next.size(); ++i)
	{
		if (prevNode->next[i].get() == curNode)
			return i;
	}

	throw DictionaryException("PrefixTreeCompressorFromEnd not found link between nodes");
}


void PrefixTreeCompressorFromEnd::get_last_phonems(t_nodes stack
													, PhonemNode * nextElem
													, t_last_nodes & mmapLastPhonems)
{
	// if phonem node the last
	// ??? TODO : may PhoneWord elements with equal id will be collapsed ???
	if (nextElem->next.size() == 0 && nextElem->get_type() == PhonemNode::NODE_TYPE_PHONEM)
	{
		put_to_last_phonems(mmapLastPhonems
							, PhonemNodeCast::to_phonem(nextElem)->get_phonem()
							, stack.back()
							, nextElem);
		return;
	}

	stack.push_back(nextElem);

	for (std::size_t i = 0; i < stack.back()->next.size(); ++i)
	{
		get_last_phonems(stack, stack.back()->next[i].get(), mmapLastPhonems);
	}

}


void PrefixTreeCompressorFromEnd::collapse_phonems(t_last_nodes & mmapLastPhonems
													, t_prev_collapsed_nodes & nextMinimization)
{
	using t_pair_nodes = std::pair<PhonemNode*, PhonemNode*>;

	if (mmapLastPhonems.size() == 0)
		return;

	auto itb = mmapLastPhonems.cbegin();
	auto ite = mmapLastPhonems.cend();

	// get the first phonem
	std::size_t ph = itb->first;
	bool is_existInSet{ false };	// is phonem in 'ph' in  'nextMinimization'
	const t_pair_nodes * nodes = &itb->second; // (prevNode, lastNode)

	while (++itb != ite)
	{
		if (itb->first == ph)
		{	// found the same phonems
			const  t_pair_nodes * collapsingNodes = &itb->second;

			auto idxNew = find_index_to_node(nodes->first, nodes->second);
			auto idxOfCollapsedElem = find_index_to_node(collapsingNodes->first, collapsingNodes->second);

			// DELETE NODE HERE
			// reassigned shared pointer, node with losing pointer will be deleted 
			collapsingNodes->first->next[idxOfCollapsedElem] = nodes->first->next[idxNew];

			nextMinimization.insert(collapsingNodes->first);
			if (!is_existInSet)
			{
				nextMinimization.insert(nodes->first);
				is_existInSet = true;
			}
		}
		else
		{
			// initialize new phonem
			ph = itb->first;
			nodes = &itb->second;
			is_existInSet = false;
		}
	}

	// 'mmapLastPhonems' may contains pointer to deleted objects
	mmapLastPhonems.clear();
}


void PrefixTreeCompressorFromEnd::find_prev_nodes_for(t_nodes stack
														, PhonemNode * nextElem
														, const t_prev_collapsed_nodes & nextMinimization
														, t_last_nodes & mmapLastPhonems)
{

	// ??? TODO : may PhoneWord elements with equal id will be collapsed ???
	if (nextMinimization.find(nextElem) != nextMinimization.end()
		&& nextElem->get_type() == PhonemNode::NODE_TYPE_PHONEM)
	{
		
		put_to_last_phonems(mmapLastPhonems
							, PhonemNodeCast::to_phonem(nextElem)->get_phonem()
							, stack.back()
							, nextElem);

		return;
	}

	stack.push_back(nextElem);

	for (std::size_t i = 0; i < stack.back()->next.size(); ++i)
	{
		find_prev_nodes_for(stack, stack.back()->next[i].get(), nextMinimization, mmapLastPhonems);
	}
}