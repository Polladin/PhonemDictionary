#pragma once

#include <memory>
#include <string>
#include <vector>


/*
	PhonemNode :

*/
class PhonemNode
{
public:
	static unsigned countOfNodes;

	enum t_node_type
	{
		NODE_TYPE_BEGIN
		, NODE_TYPE_PHONEM
		, NODE_TYPE_WORD
	};

public:

	PhonemNode() { ++countOfNodes; }

	virtual t_node_type get_type() const = 0;

	virtual ~PhonemNode() { --countOfNodes;  }


public:
	std::vector<std::shared_ptr<PhonemNode>>	next;
};



/*
	Phonem :

*/
class Phonem : public PhonemNode
{
public:

	Phonem(std::size_t i_phoneVal)
		: phoneVal(i_phoneVal)
	{}

	/*virtual*/ 
	t_node_type get_type() const override { return NODE_TYPE_PHONEM; }

	std::size_t get_phonem() const { return phoneVal; }

private:
	std::size_t phoneVal;
};


/*
	PhonemBegin :

*/
class PhonemBegin : public PhonemNode
{
public:
	/*virtual*/
	t_node_type get_type() const override { return NODE_TYPE_BEGIN; }
};



/*
	PhonemWord :

*/
class PhonemWord : public PhonemNode
{
public:
	PhonemWord(int i_wordId)
		: wordId(i_wordId)
	{}

	/*virtual*/
	t_node_type get_type() const override { return NODE_TYPE_WORD; }

	int get_val() const { return wordId; }

private:
	int wordId;
};


/*
	PhonemNodeCast:
		is used to safe reducing cast and may thow exception 'PhonemeException'
*/
class PhonemNodeCast
{
public:
	PhonemNodeCast() = delete;

	static PhonemBegin *	to_begin(PhonemNode * node);
	static Phonem *			to_phonem(PhonemNode * node);
	static PhonemWord *		to_word(PhonemNode * node);

	static const PhonemBegin *		to_begin(const PhonemNode * node);
	static const Phonem *			to_phonem(const PhonemNode * node);
	static const PhonemWord *		to_word(const PhonemNode * node);

	static bool cmp_phonem_node(Phonem * p1, Phonem * p2);
	static bool cmp_phonem_node(PhonemWord * p1, PhonemWord * p2);
};