
#include "tic_toc.hpp"

#include <unistd.h>

int main()
{
	{
		TIC_MS("1");

		usleep(1000000);
	}
	{
		TIC_US("2");

		{
			TIC("3");

			usleep(1000000);
		}

		usleep(1000000);
	}

	return 0;
}
