#pragma once

#include <memory>

#include "PhonemNode.hpp"

class PhonemNodeFactory
{
public:

	static decltype(auto) create_phonem (std::size_t val)
	{
		return std::shared_ptr<PhonemNode>(new Phonem(val));
	}

	static decltype(auto) create_word(int val)
	{
		return std::shared_ptr<PhonemNode>(new PhonemWord(val));
	}

	static decltype(auto) create_start_node()
	{
		return std::unique_ptr<PhonemNode>(new PhonemBegin());
	}
};