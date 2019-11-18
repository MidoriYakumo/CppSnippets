
#include "frame_depth.h"
#include "stack_depth.h"

#include <iostream>

void f()
{
	std::cout << "frame: " << current_frame_depth() << ", stack: " << StackCounter{}.depth()
			  << std::endl;
}

void g()
{
	StackCounter sc;

	{
		StackCounter sc;

		std::cout << "frame: " << current_frame_depth() << ", stack: " << sc.depth()
				  << std::endl;

		f();

		{
			StackCounter sc;

			std::cout << "frame: " << current_frame_depth() << ", stack: " << sc.depth()
					  << std::endl;

			f();
		}
	}
}

int main()
{
	{
		StackCounter sc;

		std::cout << "frame: " << current_frame_depth() << ", stack: " << sc.depth()
				  << std::endl;

		f();

		{
			StackCounter sc;

			std::cout << "frame: " << current_frame_depth() << ", stack: " << sc.depth()
					  << std::endl;

			g();
		}

		std::cout << "frame: " << current_frame_depth() << ", stack: " << sc.depth()
				  << std::endl;
	}

	return 0;
}
