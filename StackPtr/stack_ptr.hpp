/*

Copyright (c) 2019 Macrobull

*/

#pragma once

#include <cstddef>
#include <memory>
#include <utility>

//// hepler traits

template <bool P, typename T = void>
using enable_if_t = typename std::enable_if<P, T>::type;

template <size_t Index, typename T0, typename... Ts>
struct get_arg
{
	using type = typename get_arg<Index - 1, Ts...>::type;
};

template <typename T0, typename... Ts>
struct get_arg<0, T0, Ts...>
{
	using type = T0;
};

template <size_t Index, typename... Ts>
using get_arg_t = typename get_arg<Index, Ts...>::type;

//// storage_t

template <typename... Args>
using storage_t = typename std::aligned_union<0, Args...>::type; // union is not class

//// stack_ptr

struct placement_deleter
{
	template <typename T>
	inline void operator()(T* const pointer) const
	{
		pointer->~T();
	}
};

template <typename T>
using stack_ptr = std::unique_ptr<T, placement_deleter>;

//// create helper function

template <typename Base, typename Impl = Base,
		  /*required ahead*/
		  typename Storage, typename... CArgs>
inline stack_ptr<Base> create_stack_ptr(Storage& stack_buffer, CArgs... args)
{
	return stack_ptr<Base>{new (std::addressof(stack_buffer))
								   Impl{std::forward<CArgs>(args)...}};
}

//// route helper functions

template <size_t Index, typename Base, typename... Impls,
		  /*required ahead*/
		  typename Storage, typename... Args>
inline enable_if_t<Index + 1 == sizeof...(Impls), stack_ptr<Base>>
stack_ptr_route_impl(Storage& storage, size_t /*index*/, Args&&... args)
{
	return create_stack_ptr<Base, get_arg_t<Index, Impls...>>(
			storage, std::forward<Args>(args)...);
}

template <size_t Index, typename Base, typename... Impls,
		  /*required ahead*/
		  typename Storage, typename... Args>
inline enable_if_t<Index + 1 != sizeof...(Impls), stack_ptr<Base>>
stack_ptr_route_impl(Storage& storage, size_t index, Args&&... args)
{
	if (index == Index)
	{
		return create_stack_ptr<Base, get_arg_t<Index, Impls...>>(
				storage, std::forward<Args>(args)...);
	}

	return stack_ptr_route_impl<Index + 1, Base, Impls...>(
			storage, index, std::forward<Args>(args)...);
}

template <typename Base, typename... Impls,
		  /*required ahead*/
		  typename Storage, typename... Args>
inline stack_ptr<Base> stack_ptr_route(Storage& storage, size_t index, Args&&... args)
{
	if (index >= sizeof...(Impls))
	{
		throw std::out_of_range(std::string{"route index = "}
										.append(std::to_string(index))
										.append(" out of range"));
	}

	return stack_ptr_route_impl<0, Base, Impls...>(storage, index, std::forward<Args>(args)...);
}
