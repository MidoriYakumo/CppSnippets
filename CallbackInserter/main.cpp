
#include "callback_inserter.hpp"

#include <cmath>
#include <iostream>
#include <tuple>
#include <vector>

void f(int x)
{
	std::cout << "f: " << x << " ";
}

int main()
{
	{
		std::vector<int> a{
				1, 2, 3, 4, 5,
		};
		std::copy(a.begin(), a.end(), CallbackInsertIterator<void, int>{f});
		std::cout << std::endl;
		std::copy(a.begin(), a.end(), callback_inserter(f));
		std::cout << std::endl;
		std::copy(a.begin(), a.end(),
				  callback_inserter([](int x) { std::cout << ++x << " "; }));
		std::cout << std::endl;
		std::copy(a.begin(), a.end(),
				  callback_inserter([](int& x) { std::cout << ++x << " "; }));
		std::cout << std::endl;
		std::copy(a.begin(), a.end(),
				  callback_inserter([](int& x) { std::cout << ++x << " "; }));
		std::cout << std::endl;
	}
	{
		std::vector<int> a{
				1, 2, 3, 4, 5,
		};
		std::vector<std::tuple<int, int&>> b{
				{0, a[0]}, {1, a[1]}, {2, a[2]}, {3, a[3]}, {4, a[4]},
		};
		std::copy(b.begin(), b.end(), callback_inserter([](std::tuple<int, int&> x) {
					  std::cout << ++std::get<0>(x) << " " << ++std::get<1>(x) << " ";
				  }));
		std::cout << std::endl;
		std::copy(b.begin(), b.end(), callback_inserter([](std::tuple<int, int&> x) {
					  std::cout << ++std::get<0>(x) << " " << ++std::get<1>(x) << " ";
				  }));
		std::cout << std::endl;
	}
	{
		std::vector<float> a{
				1, 2, 3, 4, 5,
		};
		std::copy(a.begin(), a.end(), callback_inserter(std::function<void(float)>{fabs}));
	}
}
