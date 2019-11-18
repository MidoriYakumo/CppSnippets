
#include "cast.hpp"

#include <cstdint>
#include <iostream>

int main()
{
	{
		int a = -8, b = 66666;
		std::cout << cast<uint16_t>(a) << " " << cast<uint16_t>(b) << std::endl;
	}
	{
		uint8_t a[] = {0, 0, 128, 63};
		std::cout << *cast<float*>(a) << std::endl;
	}
	return 0;
}
