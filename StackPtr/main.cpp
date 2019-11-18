
#include "stack_ptr.hpp"

#include <iostream>

struct A
{
	virtual ~A()
	{
		std::cout << "~A" << std::endl;
	}

	virtual std::string f()
	{
		return "A";
	}
};

struct B : A
{
	std::string f() override
	{
		return "B";
	}
};

struct C : A
{
	~C() override
	{
		std::cout << "~C" << std::endl;
	}

	std::string f() override
	{
		return "C";
	}
};

int main()
{
	storage_t<A, B, C> stack_buffer;

	try
	{
		{
			stack_ptr<A> ptr;
			int          choice = 1;
			switch (choice)
			{
				case 1:
					ptr = create_stack_ptr<A, B>(stack_buffer);
					break;
				case 2:
					ptr = create_stack_ptr<A, C>(stack_buffer);
					break;
				default:
					ptr = create_stack_ptr<A>(stack_buffer);
			}
			std::cout << ptr->f() << std::endl;
		}

		for (int i = 0; i < 3; ++i)
		{
			auto ptr = stack_ptr_route<A, A, B, C>(stack_buffer, i);
			std::cout << ptr->f() << std::endl;
		}
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}
	return 0;
}
