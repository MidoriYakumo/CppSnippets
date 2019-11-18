
#include "func_traits.hpp"

#include <cmath>
#include <functional>
#include <iostream>

float f(int x)
{
	return float(x);
}

int main()
{
	std::cout << std::is_same<float, result_type<decltype(f)>>::value << std::endl;
	auto lambda = [](int x, int y) { return x + y; };
	std::cout << std::is_same<int, result_type<decltype(lambda)>>::value << std::endl;
	std::cout << std::is_same<float, result_type<decltype(fabs)>>::value << std::endl;
	std::cout << std::is_same<double, result_type<decltype(fabs)>>::value << std::endl;
	std::function<float(float)> func{fabs};
	std::cout << std::is_same<float, result_type<decltype(func)>>::value << std::endl;
	std::cout << std::is_same<std::tuple<float>, params_type<decltype(func)>>::value
			  << std::endl;
	std::cout << func(3.f) << std::endl;
	return 0;
}
