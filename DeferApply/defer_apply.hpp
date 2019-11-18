/*

Copyright (c) 2019 Macrobull

*/

#pragma once

#include <functional>
#include <tuple>
#include <utility>

//// hepler traits

template <bool P, typename T = void>
using enable_if_t = typename std::enable_if<P, T>::type;

template <typename T>
using result_of_t = typename std::result_of<T>::type;

//// direct apply

template <typename F, typename... Args>
inline result_of_t<F(Args...)> apply(const F& func, Args&&... args)
{
	return func(std::forward<Args>(args)...);
}

template <typename Ret, typename... Args>
inline Ret apply(Ret(&&func)(Args...), Args&&... args)
{
	return func(std::forward<Args>(args)...);
}

//// Defer: functor class stores a function with its default parameters

template <typename Ret, typename... Params>
struct Defer
{
	std::function<Ret(Params...)> func;
	std::tuple<Params...>         params;

	template <typename... Args>
	inline Ret operator()(Args&&... args) const
	{
		return dispatch(std::forward<Args>(args)...);
	}

	template <typename... Args>
	inline Ret operator()(Args&&... args)
	{
		return dispatch(std::forward<Args>(args)...);
	}

	template <typename... Args>
	inline enable_if_t<sizeof...(Args) != sizeof...(Params), Ret> dispatch(Args&&... args) const
	{
		return dispatch(std::forward<Args>(args)..., std::get<sizeof...(Args)>(params));
	}

	template <typename... Args>
	inline enable_if_t<sizeof...(Args) != sizeof...(Params), Ret> dispatch(Args&&... args)
	{
		return dispatch(std::forward<Args>(args)..., std::get<sizeof...(Args)>(params));
	}

	inline Ret dispatch(Params... args) const
	{
		return func(std::forward<Params>(args)...);
	}

	inline Ret dispatch(Params... args)
	{
		return func(std::forward<Params>(args)...);
	}
};

//// make template function make_defer

template <typename Ret, typename... Args>
inline Defer<Ret, Args...> make_defer(Ret(func)(Args...), std::tuple<Args...> args)
{
	return Defer<Ret, Args...>{std::move(func), std::move(args)};
}

template <typename Ret, typename... CArgs>
inline Defer<Ret, CArgs...> make_defer(Ret(func)(CArgs...), CArgs&&... args)
{
	return Defer<Ret, CArgs...>{
			std::move(func), std::move(std::make_tuple(std::forward<CArgs>(args)...))};
}

template <typename F, typename... Args>
inline Defer<result_of_t<F(Args...)>, Args...> make_defer(F func, std::tuple<Args...> args)
{
	using Ret = result_of_t<F(Args...)>;

	return Defer<Ret, Args...>{std::move(func), std::move(args)};
}

template <typename F, typename... CArgs>
inline Defer<result_of_t<F(CArgs...)>, CArgs...> make_defer(F func, CArgs&&... args)
{
	using Ret = result_of_t<F(CArgs...)>;

	return Defer<Ret, CArgs...>{
			std::move(func), std::move(std::make_tuple(std::forward<CArgs>(args)...))};
}

//// apply with argument tuple

template <typename Ret, typename... Args>
inline Ret apply(Ret(func)(Args...), std::tuple<Args...> args)
{
	return Defer<Ret, Args...>{std::move(func), std::move(args)}();
}

template <typename F, typename... Args>
inline result_of_t<F(Args...)> apply(F func, std::tuple<Args...> args)
{
	return Defer<result_of_t<F(Args...)>, Args...>{std::move(func), std::move(args)}();
}

//// helper functions for constructor-apply specialization

template <typename T /*required ahead*/, typename... CArgs>
inline T construct(CArgs... args)
{
	return T{std::forward<CArgs>(args)...};
}

template <typename T /*required ahead*/, typename... Args>
inline T construct(std::tuple<Args...>&& args)
{
	return apply<T, Args...>(construct, std::forward<std::tuple<Args...>>(args));
}

template <typename T /*required ahead*/, typename... CArgs>
inline T* create(CArgs... args)
{
	return new T{std::forward<CArgs>(args)...};
}

template <typename T /*required ahead*/, typename... Args>
inline T* create(std::tuple<Args...>&& args)
{
	return apply<T*, Args...>(create, std::forward<std::tuple<Args...>>(args));
}

//// apply-reconstruct

#include <memory>

template <typename T, typename... CArgs>
inline T& reconstruct(T& target, CArgs... args)
{
	target.~T();
	new (std::addressof(target)) T{std::forward<CArgs>(args)...};
	return target;
}

template <typename T, typename... Args>
inline T& reconstruct(T& target, std::tuple<Args...>&& args)
{
	return apply<T&, T&, Args...>(
			reconstruct,
			std::forward<std::tuple<T&, Args...>>(std::tuple_cat(
					std::tuple<T&>(target), std::forward<std::tuple<Args...>>(args))));
}
