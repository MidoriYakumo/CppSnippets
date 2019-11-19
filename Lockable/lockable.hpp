/*

Copyright (c) 2019 Macrobull

*/

#pragma once

#include <mutex>

template <typename T>
class Lockable : public T
{
public:
	class Locked
	{
	public:
		explicit Locked(Lockable& parent)
			: m_parent{parent}
		{
			m_parent.lock();
		}

		~Locked()
		{
			m_parent.unlock(); // throw ?
		}

		Locked(const Locked&)     = delete;
		Locked(Locked&&) noexcept = default;

		Locked& operator=(const Locked&) = delete;

	private:
		Lockable& m_parent;
	};

	using T::T;

	inline void lock()
	{
		m_lock.lock();
	}

	inline bool try_lock() noexcept
	{
		return m_lock.try_lock();
	}

	inline void unlock()
	{
		m_lock.unlock();
	}

	inline Locked guard()
	{
		return Locked{*this};
	}

private:
	std::mutex m_lock; // HINT: noncopyable
};

#define LOCKABLE_VARNAME_CONCAT(base, line) base##line
#define LOCKABLE_VARNAME(line) LOCKABLE_VARNAME_CONCAT(tic_, line)
#define LOCKABLE_GUARD(lockable) const auto LOCKABLE_VARNAME(__LINE__) {(lockable).guard()}

template <typename T>
class Singleton
{
public:
	template <typename... CArgs>
	inline static T& instance(CArgs... args)
	{
		static T ret{std::forward<CArgs>(args)...};
		return ret;
	}

	Singleton(const Singleton&) = delete;

	Singleton& operator=(const Singleton&) = delete;

protected:
	Singleton() = default;

	~Singleton() = default;
};

template <typename T>
using LockableSingleton = Singleton<Lockable<T>>;
