#include "atta/atta.h"

int main(int argc, char *argv[])
{
	if(argc == 1)
	{
		atta::Atta atta;
		atta.run();
	}
	else
	{
		if(std::string(argv[1]) == "--version" || std::string(argv[1]) == "-v")
		{
			std::cout << "Atta Simulator 0.0.2.0" <<
				"\nThis project is licensed under the MIT License\n";
		}
	}

	return EXIT_SUCCESS;
}
