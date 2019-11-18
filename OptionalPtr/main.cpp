
#include "optional_ptr.hpp"

#include <iostream>
#include <string>
#include <vector>

std::string f(const optional_ptr<std::string>& x = nullptr)
{
	x->append(std::string{x.fallback() ? "" : " "}.append("hello"));
	return *x;
}

std::size_t g(std::vector<int>* x_ = nullptr)
{
	const optional_ptr<std::vector<int>> x{x_, std::vector<int>(1, 1)};
	x->resize(x->size() * 2);
	return x->size();
}

std::size_t h(std::vector<int>* x_ = nullptr)
{
	const optional_ptr<std::vector<int>> x{x_, 1, 1};
	x->resize(x->size() * 2);
	return x->size();
}

int main()
{
	{
		int* x = new int{3};
		DECLARE_OPT_PTR_FOR(x, 1);
		std::cout << *x_ptr << std::endl;
		delete x;
	}
	{
		int* x = nullptr;
		DECLARE_OPT_PTR_FOR(x, 1);
		std::cout << *x_ptr << std::endl;
		delete x;
	}
	{
		std::string x{"c++"};
		std::cout << f() << std::endl;
		std::cout << f(&x) << std::endl;
		std::vector<int> y = {1, 2, 3};
		std::cout << g() << std::endl;
		std::cout << g(&y) << std::endl;
		std::cout << h() << std::endl;
		std::cout << h(&y) << std::endl;
	}
	return 0;
}
