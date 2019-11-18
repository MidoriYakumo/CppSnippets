
#include "lockable.hpp"

#include <iostream>
#include <thread>
#include <vector>

struct A
{
	explicit A(int a)
		: x{a}
		, y{a}
	{}

	A(int a, int b)
		: x{a}
		, y{b}
	{}

	int x, y;
};

struct B : Singleton<B>
{
	int get_x() const
	{
		return x;
	}

	int get_y() const
	{
		return y;
	}

protected:
	friend class Singleton<B>;

	explicit B(int a)
		: x{a}
		, y{a}
	{}

	B(int a, int b)
		: x{a}
		, y{b}
	{}

private:
	int x, y;
};

int main()
{
	std::cout << Singleton<A>::instance(1).x << std::endl;
	std::cout << B::instance(2, 3).get_x() << std::endl;

	std::vector<std::thread> threads;
	threads.reserve(9);
	for (int i = 0; i < 9; ++i)
	{
		threads.emplace_back([i]() {
			const auto a_guard{LockableSingleton<A>::instance((i + 1) * 10 + 1).lock_guard()};

			std::cout << "thread " << i << ": " << LockableSingleton<A>::instance(8).x
					  << std::endl;
			std::cout << "thread " << i << ": " << LockableSingleton<A>::instance(-8).try_lock()
					  << std::endl;
		});
	}
	for (int i = 0; i < 9; ++i)
	{
		threads[i].join();
	}
	threads.clear();

	std::cout << std::endl;
	for (int i = 0; i < 9; ++i)
	{
		threads.emplace_back([i]() {
			const Lockable<A>::Locked a_guard{Singleton<Lockable<A>>::instance(0)};

			Singleton<Lockable<A>>::instance(8).x = i;
			std::cout << "thread " << i << ": " << Singleton<Lockable<A>>::instance(-8).x
					  << std::endl;
		});
	}
	for (int i = 0; i < 9; ++i)
	{
		threads[i].join();
	}

	return 0;
}
