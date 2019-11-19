/*

Copyright (c) 2019 Macrobull

*/

#pragma once

#include <type_traits>

//// macro version

// CHECKIT: remove_reference or even decay ?
#define DECLARE_OPT_PTR_FOR(name, ...)                                                         \
	typename std::remove_reference<decltype(*name)>::type name##_fallback{__VA_ARGS__};        \
	auto name##_ptr = (name != nullptr) ? name : &name##_fallback

//// template class version

#include <memory>

//// optional_ptr

template <typename T>
class optional_ptr
{
public:
	template <typename... CArgs>
	/*explicit*/ optional_ptr(T* const pointer, CArgs... args) // accept implicit conversion
		: m_pointer{(pointer == nullptr) ? new (m_storage) T{std::move(args)...} : pointer}
		, m_fallback{pointer == nullptr}
	{}

	virtual ~optional_ptr();

	optional_ptr(const optional_ptr&) = delete;

	optional_ptr& operator=(const optional_ptr&) = delete;

	inline T& operator*() const noexcept
	{
		return *m_pointer;
	}

	inline T* operator->() const noexcept
	{
		return m_pointer;
	}

	inline bool fallback() const noexcept
	{
		return m_fallback;
	}

private:
	//	union
	//	{
	//		T m_payload;
	//	};
	char       m_storage[sizeof(T)]{};
	T* const   m_pointer{};
	const bool m_fallback{};
};

//// implementation

template <typename T>
optional_ptr<T>::~optional_ptr()
{
	if (m_fallback)
	{
		m_pointer->~T();
	}
}
