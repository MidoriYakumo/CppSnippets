/*

Copyright (c) 2019 Macrobull

*/

#pragma once

#include "stack_depth.h"

#ifdef STACK_DEPTH_PRIVATE_IMPL

#define STACK_DEPTH_IMPL_STORAGE

#else

#define STACK_DEPTH_IMPL_STORAGE inline

#endif

namespace detail
{

inline size_t& depth()
{
	static thread_local size_t ret{0};
	return ret;
}

} // namespace detail

STACK_DEPTH_IMPL_STORAGE StackCounter::StackCounter()
{
	++(detail::depth());
}

STACK_DEPTH_IMPL_STORAGE StackCounter::~StackCounter()
{
	--(detail::depth());
}

STACK_DEPTH_IMPL_STORAGE size_t StackCounter::depth() const
{
	return detail::depth();
}
