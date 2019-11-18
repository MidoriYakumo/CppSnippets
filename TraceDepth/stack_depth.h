/*

Copyright (c) 2019 Macrobull

*/

#pragma once

#include <cstddef>

//// StackCounter: RAII class counting the stack depth

struct StackCounter
{
	StackCounter();
	~StackCounter();

	StackCounter(const StackCounter&) = delete;
	StackCounter(StackCounter&&)      = default;

	StackCounter& operator=(const StackCounter&) = delete;

	size_t depth() const;
};
