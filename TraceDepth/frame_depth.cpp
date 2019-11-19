/*

Copyright (c) 2019 Macrobull

*/

#include "frame_depth.h"

#include <cassert>
#include <unwind.h>

namespace
{

_Unwind_Reason_Code get_one_frame(struct _Unwind_Context * /*uc*/, void *opaque) // noexcept
{
	auto depth = reinterpret_cast<size_t *>(opaque);
	++(*depth);
	return _URC_NO_REASON;
}

_Unwind_Reason_Code nop_backtrace(struct _Unwind_Context * /*uc*/, void * /*opaque*/) noexcept
{
	return _URC_NO_REASON;
}

bool ready_to_run{false};

struct TraceInit
{
	TraceInit() noexcept
	{
		_Unwind_Backtrace(nop_backtrace, nullptr);
		ready_to_run = true;
	}
};

TraceInit _;

} // namespace

size_t current_frame_depth()
{
	size_t depth{0};

	if (!ready_to_run)
	{
		return depth;
	}

	_Unwind_Backtrace(get_one_frame, &depth);
	assert(depth > 0);

	return depth - 1;
}
