
#include "stack_storage.hpp"

#include <iostream>

struct A // Trivial Copyable
{
	explicit A(int a)
		: x{a}
		, y{a}
	{}

	A(int a, int b)
		: x{a}
		, y{b}
	{}

	~A()
	{
		std::cout << "~A()" << std::endl;
	}

	int x, y;
};

struct B // NonCopyable
{
	explicit B(int a)
		: x{a}
		, y{a}
	{}

	B(int a, int b)
		: x{a}
		, y{b}
	{}

	~B()
	{
		std::cout << "~B()" << std::endl;
	}

	B(const B&) = delete;
	B& operator=(const B&) = delete;

	int x, y;
};

int main()
{
	{
		stack_storage<A> x{1, 2};
		std::cout << x->x << ' ' << x->y << std::endl;
		x.construct(3);
		std::cout << (*x).x << ' ' << x->y << std::endl;
		auto y = x;
		std::cout << y->x << ' ' << y->y << std::endl;
		stack_storage<A> z;
		z = x;
		std::cout << z->x << ' ' << z->y << std::endl;
	}
	{
		stack_storage<B> x;
		std::cout << x.inited() << std::endl;
		x.construct(4);
		std::cout << x->x << ' ' << x->y << std::endl;
	}
	return 0;
}
