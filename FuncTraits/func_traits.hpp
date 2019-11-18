/*

Copyright (c) 2019 Macrobull

*/

#pragma once

#include <tuple>

//// func_traits: params_type & result_type for function

// forward declaration
template <typename T>
struct func_traits;

// static function
template <typename Ret, typename... Args>
struct func_traits<Ret(Args...)>
{
	using params_type = std::tuple<Args...>;
	using result_type = Ret;
};

// static function pointer
template <typename Ret, typename... Args>
struct func_traits<Ret (*)(Args...)>
{
	using params_type = std::tuple<Args...>;
	using result_type = Ret;
};

// member function
template <typename T, typename Ret, typename... Args>
struct func_traits<Ret (T::*)(Args...) const>
{
	using params_type = std::tuple<Args...>;
	using result_type = Ret;
};

// member function
template <typename T, typename Ret, typename... Args>
struct func_traits<Ret (T::*)(Args...)>
{
	using params_type = std::tuple<Args...>;
	using result_type = Ret;
};

// lambda function
template <typename F>
struct func_traits : func_traits<decltype(&F::operator())>
{
};

//// sub-type helpers

template <typename T>
using params_type = typename func_traits<T>::params_type;

template <typename T>
using result_type = typename func_traits<T>::result_type;
