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
			m_parent.unlock();
		}

		Locked(const Locked&)     = delete;
		Locked(Locked&&) noexcept = default;

		Locked& operator=(const Locked&) = delete;

	private:
		Lockable& m_parent;
	};

	using T::T;

	void lock()
	{
		m_lock.lock();
	}

	bool try_lock()
	{
		return m_lock.try_lock();
	}

	void unlock()
	{
		m_lock.unlock();
	}

	Locked lock_guard()
	{
		return Locked{*this};
	}

private:
	std::mutex m_lock; // HINT: noncopyable
};

template <typename T>
class Singleton
{
public:
	template <typename... CArgs>
	static T& instance(CArgs... args)
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
