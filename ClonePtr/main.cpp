
#include "clone_ptr.inl.hpp"

#include <iostream>

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
	{
		ClonePtr<B> x{1};
		auto        y = x;
		y->y          = 3;
		auto z        = y;
		std::cout << x->x << ", " << x->y << std::endl;
		std::cout << y->x << ", " << y->y << std::endl;
		std::cout << z->x << ", " << z->y << std::endl;
	}
	{
		ClonePtr<B> x{1, 2};
		auto        y = x;
		y->y          = 3;
		auto z        = y;
		std::cout << x->x << ", " << x->y << std::endl;
		std::cout << y->x << ", " << y->y << std::endl;
		std::cout << z->x << ", " << z->y << std::endl;

		std::cout << y->x << ", " << y->y << std::endl;
		y.reconstruct();
		std::cout << y->x << ", " << y->y << std::endl;
	}
	return 0;
}
