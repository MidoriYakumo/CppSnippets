
#include "persist_stream.h"

#include <iostream>

int main()
{
	PersistOutStream ostream;

	ostream.reset(std::cout) << "Hello stdout" << std::endl;
	(ostream = std::cerr) << "Hello stderr" << std::endl;
	ostream << "Again stderr" << std::endl;

	return 0;
}
