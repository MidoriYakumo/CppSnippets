/*

Copyright (c) 2019 Macrobull

*/

#pragma once

#include <cstddef>
#include <limits>
#include <utility>

#ifdef DEBUG

#include <cassert>

#endif

//#define CAST_SATURATED_DEBUG_ONLY
//#define CAST_DYNAMIC_DEBUG_ONLY

//// helper traits

template <bool P, typename T = void>
using enable_if_t = typename std::enable_if<P, T>::type;

template <typename T>
using decay_t = typename std::decay<T>::type;

template <typename T>
using remove_pointer_t = typename std::remove_pointer<T>::type;

// dataview type trait
template <typename T>
// using is_data_type = std::is_integral<T>;
using is_data_type = std::is_arithmetic<T>;

//// saturated_cast for numeric casting

template <typename T /*required ahead*/, typename U>
T saturated_cast(U value)
{
	constexpr auto min_narrowed = std::numeric_limits<T>::min() > std::numeric_limits<U>::min();
	constexpr auto max_narrowed = std::numeric_limits<T>::max() < std::numeric_limits<U>::max();
	constexpr auto min_t_in_u   = static_cast<U>(std::numeric_limits<T>::min());
	constexpr auto max_t_in_u   = static_cast<U>(std::numeric_limits<T>::max());

	return static_cast<T>(max_narrowed && value > max_t_in_u
								  ? max_t_in_u
								  : min_narrowed && value < min_t_in_u ? min_t_in_u : value);
}

//// for numbers, use saturated_cast

template <typename T /*required ahead*/, typename U>
enable_if_t<std::is_arithmetic<T>::value && std::is_arithmetic<U>::value, T>
cast(U value)
{
#if !defined(CAST_SATURATED_DEBUG_ONLY) || (defined(DEBUG) && !defined(NDEBUG))
#if defined(CAST_SATURATED_DEBUG_ONLY) && defined(assert)

	const auto ret = saturated_cast<T>(value);
	assert(ret == static_cast<T>(value) && "using static_cast may be unsafe");
	return ret;

#else

	return saturated_cast<T>(value);

#endif
#else

	return static_cast<T>(value);

#endif
}

//// for data view pointers, use static_cast via void *

template <typename T /*required ahead*/, typename U>
enable_if_t<is_data_type<remove_pointer_t<T>>::value && is_data_type<U>::value, T>
cast(const U* value)
{
	return static_cast<T>(static_cast<const void*>(value));
}

template <typename T /*required ahead*/, typename U>
enable_if_t<is_data_type<remove_pointer_t<T>>::value && is_data_type<U>::value, T>
cast(U* value)
{
	return static_cast<T>(static_cast<void*>(value));
}

template <typename T /*required ahead*/, typename U, size_t N>
enable_if_t<is_data_type<remove_pointer_t<T>>::value && is_data_type<U>::value, T>
cast(const U value[N])
{
	return static_cast<T>(static_cast<const void*>(value));
}

template <typename T /*required ahead*/, typename U, size_t N>
enable_if_t<is_data_type<remove_pointer_t<T>>::value && is_data_type<U>::value, T>
cast(U value[N])
{
	return static_cast<T>(static_cast<void*>(value));
}

//// dynamic_cast for polymorphic pointers

template <typename T /*required ahead*/, typename U>
enable_if_t<std::is_polymorphic<remove_pointer_t<T>>::value && std::is_polymorphic<U>::value, T>
cast(const U* value)
{
#if !defined(CAST_DYNAMIC_DEBUG_ONLY) || (defined(DEBUG) && !defined(NDEBUG))
#if defined(CAST_DYNAMIC_DEBUG_ONLY) && defined(assert)

	const auto ret = dynamic_cast<T>(value);
	assert(ret != nullptr && "using static_cast may be unsafe");
	return ret;

#else

	return dynamic_cast<T>(value);

#endif
#else

	return static_cast<T>(value);

#endif
}

template <typename T /*required ahead*/, typename U>
enable_if_t<std::is_polymorphic<remove_pointer_t<T>>::value && std::is_polymorphic<U>::value, T>
cast(U* value)
{
#if !defined(CAST_DYNAMIC_DEBUG_ONLY) || (defined(DEBUG) && !defined(NDEBUG))
#if defined(CAST_DYNAMIC_DEBUG_ONLY) && defined(assert)

	const auto ret = dynamic_cast<T>(value);
	assert(ret != nullptr && "using static_cast may be unsafe");
	return ret;

#else

	return dynamic_cast<T>(value);

#endif
#else

	return static_cast<T>(value);

#endif
}

//// otherwise directly static_cast

template <typename T /*required ahead*/, typename U>
enable_if_t<!((std::is_arithmetic<T>::value && std::is_arithmetic<U>::value) ||
			  (is_data_type<remove_pointer_t<T>>::value &&
			   is_data_type<remove_pointer_t<decay_t<U>>>::value) ||
			  (std::is_polymorphic<remove_pointer_t<T>>::value &&
			   std::is_polymorphic<remove_pointer_t<decay_t<U>>>::value)),
			T>
cast(U&& value)
{
	return static_cast<T>(std::forward<U>(value));
}
