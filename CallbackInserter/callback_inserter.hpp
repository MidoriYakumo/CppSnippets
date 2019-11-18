/*

Copyright (c) 2019 Macrobull

*/

#pragma once

#include <functional>
#include <utility>

//// helper traits

template <typename T>
using decay_t = typename std::decay<T>::type;

//// AssignFunctor: functor class can be called with operator=

// TypeA: return self
template <typename Ret, typename... Params>
struct AssignFunctorTypeA : public std::function<Ret(Params...)>
{
	using Base = std::function<Ret(Params...)>;

	using Base::Base;

	explicit AssignFunctorTypeA(Base func)
		: Base{std::move(func)}
	{}

	template <typename... Args>
	inline const AssignFunctorTypeA& operator=(Args&&... args) const
	{
		Base::operator()(std::forward<Args>(args)...);
		return *this;
	}

	template <typename... Args>
	inline AssignFunctorTypeA& operator=(Args&&... args)
	{
		Base::operator()(std::forward<Args>(args)...);
		return *this;
	}
};

// TypeB: return ret
template <typename Ret, typename... Params>
struct AssignFunctorTypeB : public std::function<Ret(Params...)>
{
	using Base = std::function<Ret(Params...)>;

	using Base::Base;

	explicit AssignFunctorTypeB(Base func)
		: Base{std::move(func)}
	{}

	template <typename... Args>
	inline Ret operator=(Args&&... args) const
	{
		return Base::operator()(std::forward<Args>(args)...);
	}

	template <typename... Args>
	inline Ret operator=(Args&&... args)
	{
		return Base::operator()(std::forward<Args>(args)...);
	}
};

// type selector
template <typename Ret, typename... Params>
using AssignFunctor = AssignFunctorTypeB<Ret, Params...>;

//// CallbackInsertIterator: an iterator class yielding AssignFunctor

template <typename Ret, typename... Params>
struct CallbackInsertIterator
{
	using CallbackType = std::function<Ret(Params...)>;
	using FunctorType  = AssignFunctor<Ret, Params...>;

	FunctorType func;

	explicit CallbackInsertIterator(CallbackType callback)
		: func{std::move(callback)}
	{}

	inline const CallbackInsertIterator& operator++() const
	{
		return *this;
	}

	inline CallbackInsertIterator& operator++()
	{
		return *this;
	}

	inline const FunctorType& operator*() const
	{
		return func;
	}

	inline FunctorType& operator*()
	{
		return func;
	}
};

//// make iterator_traits for CallbackInsertIterator

// hit cert-dcl58-cpp
namespace std
{

template <typename Ret, typename... Params>
struct iterator_traits<CallbackInsertIterator<Ret, Params...>>
{
	using value_type = void; //
};

} // namespace std

//// make template function callback_inserter

// forward declaration
template <typename T>
struct CallbackInserterHelper;

// static function pointer
template <typename Ret, typename... Params>
struct CallbackInserterHelper<Ret (*)(Params...)>
{
	using IteratorType = CallbackInsertIterator<Ret, Params...>;

	inline static IteratorType create(typename IteratorType::CallbackType callback)
	{
		return IteratorType{std::move(callback)};
	}
};

// member function
template <typename T, typename Ret, typename... Params>
struct CallbackInserterHelper<Ret (T::*)(Params...) const>
{
	using IteratorType = CallbackInsertIterator<Ret, Params...>;

	inline static IteratorType create(typename IteratorType::CallbackType callback)
	{
		return IteratorType{std::move(callback)};
	}
};

// member function
template <typename T, typename Ret, typename... Params>
struct CallbackInserterHelper<Ret (T::*)(Params...)>
{
	using IteratorType = CallbackInsertIterator<Ret, Params...>;

	inline static IteratorType create(typename IteratorType::CallbackType callback)
	{
		return IteratorType{std::move(callback)};
	}
};

// lambda function
template <typename F>
struct CallbackInserterHelper : CallbackInserterHelper<decltype(&F::operator())>
{};

// the template function
template <typename T>
inline typename CallbackInserterHelper<decay_t<T>>::IteratorType
callback_inserter(const T& callback)
{
	return CallbackInserterHelper<decay_t<T>>::create(callback);
}
