
#include "defer_apply.hpp"

#include <cmath>
#include <iostream>

struct A // Trivial
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

	B(const B&) = delete;

	B& operator=(const B&) = delete;

	int x, y;
};

int main()
{
	std::cout << Defer<float, float>{fabs, -3.f}() << std::endl;
	std::cout << Defer<float, float>{fabs, std::make_tuple(-3.f)}() << std::endl;
	std::cout << Defer<float, float>{[](float x) { return -x; }, std::make_tuple(-3.f)}()
			  << std::endl;
	std::cout << make_defer(fabs, -3.)() << std::endl;
	std::cout << make_defer([](int x) { return x + 1; }, 2)() << std::endl;
	std::cout << apply(fabs, -3.) << std::endl;
	std::cout << apply([](float x) { return -x; }, -3.f) << std::endl;
	std::cout << apply([](int x, int y) { return A(x, y); }, 1, 2).x << std::endl;

	std::cout << construct<A>(1, 2).x << std::endl;
	std::cout << construct<A>(std::make_tuple(1, 2)).x << std::endl;
	std::cout << create<B>(std::make_tuple(1, 2))->x << std::endl;

	{
		B x{1, 2};
		std::cout << x.x << std::endl;
		reconstruct(x, 3, 4);
		std::cout << x.x << std::endl;
		reconstruct(x, std::make_tuple(5, 6));
		std::cout << x.x << std::endl;
	}
}
