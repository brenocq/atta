#include "atta/atta.h"

int main(int argc, char *argv[])
{
	atta::Atta::CreateInfo info {};
	if(argc == 1)
	{
	}
	else
	{
		if(std::string(argv[1]) == "--version" || std::string(argv[1]) == "-v")
		{
			std::cout << "Atta Simulator 0.0.2.0" <<
				"\nThis project is licensed under the MIT License\n";
			return EXIT_SUCCESS;
		}
		else
		{
			fs::path attaFile(argv[1]);
			info.projectFile = attaFile;
		}
	}

	atta::Atta atta(info);
	atta.run();

	return EXIT_SUCCESS;
}
