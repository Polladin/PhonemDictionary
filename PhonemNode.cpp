
#include "PhonemNode.hpp"

#include "DictionaryException.hpp"

unsigned PhonemNode::countOfNodes{ 0 };



PhonemBegin * PhonemNodeCast::to_begin(PhonemNode * node)
{
	if (node->get_type() != PhonemNode::NODE_TYPE_BEGIN)
		throw DictionaryException("Bad Cast to PhonemBegin("
											+ std::to_string(PhonemNode::NODE_TYPE_BEGIN) 
											+ ") from "
											+ std::to_string(node->get_type()) );

	return static_cast<PhonemBegin*>(node);
}


Phonem * PhonemNodeCast::to_phonem(PhonemNode * node)
{
	if (node->get_type() != PhonemNode::NODE_TYPE_PHONEM)
		throw DictionaryException("Bad Cast to Phonem("
											+ std::to_string(PhonemNode::NODE_TYPE_PHONEM)
											+ ") from "
											+ std::to_string(node->get_type()));

	return static_cast<Phonem*>(node);
}


PhonemWord * PhonemNodeCast::to_word(PhonemNode * node)
{
	if (node->get_type() != PhonemNode::NODE_TYPE_WORD)
		throw DictionaryException("Bad Cast to PhonemWord("
											+ std::to_string(PhonemNode::NODE_TYPE_WORD)
											+ ") from "
											+ std::to_string(node->get_type()));

	return static_cast<PhonemWord*>(node);
}


const PhonemBegin * PhonemNodeCast::to_begin(const PhonemNode * node)
{
	if (node->get_type() != PhonemNode::NODE_TYPE_BEGIN)
		throw DictionaryException("Bad Cast to PhonemBegin("
			+ std::to_string(PhonemNode::NODE_TYPE_BEGIN)
			+ ") from "
			+ std::to_string(node->get_type()));

	return static_cast<const PhonemBegin*>(node);
}


const Phonem * PhonemNodeCast::to_phonem(const PhonemNode * node)
{
	if (node->get_type() != PhonemNode::NODE_TYPE_PHONEM)
		throw DictionaryException("Bad Cast to Phonem("
			+ std::to_string(PhonemNode::NODE_TYPE_PHONEM)
			+ ") from "
			+ std::to_string(node->get_type()));

	return static_cast<const Phonem*>(node);
}


const PhonemWord * PhonemNodeCast::to_word(const PhonemNode * node)
{
	if (node->get_type() != PhonemNode::NODE_TYPE_WORD)
		throw DictionaryException("Bad Cast to PhonemWord("
			+ std::to_string(PhonemNode::NODE_TYPE_WORD)
			+ ") from "
			+ std::to_string(node->get_type()));

	return static_cast<const PhonemWord*>(node);
}