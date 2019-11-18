
#include "reconstructable.hpp"

#include <iostream>

struct B // B
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
		B x{1, 2};
		std::cout << x.x << ", " << x.y << std::endl;
		ReconstructorImpl<B, int, int>{std::make_tuple(3, 4)}.reconstruct(x);
		std::cout << x.x << ", " << x.y << std::endl;
	}
	{
		Reconstructable<B> x{1, 2};
		x.x = 3;
		x.y = 4;
		std::cout << x.x << ", " << x.y << std::endl;
		x.reconstruct();
		std::cout << x.x << ", " << x.y << std::endl;
	}
	{
		ReconstructableImpl<B> x{true, 1, 2}; // will raise assertion error
		x.x = 3;
		x.y = 4;
		std::cout << x.x << ", " << x.y << std::endl;
		x.reconstruct();
		std::cout << x.x << ", " << x.y << std::endl;
	}
	return 0;
}
