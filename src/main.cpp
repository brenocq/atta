#include <iostream>
#include <atta/atta.h>
#include <atta/helpers/log.h>
//#include "examples/atta.h"

void welcome();

int main()
{
	welcome();

	atta::Atta::CreateInfo attaInfo {
		.dimensionMode = atta::DIM_MODE_3D,
		.physicsMode = atta::PHY_MODE_DISABLED,
		.createWindow = true,
	};
	atta::Atta atta(attaInfo);
	atta.run();
	return EXIT_SUCCESS;
}

void welcome()
{
	std::cout << 
		COLOR_BOLD_GREEN << 
		"    ┌───────────────────────────────────┐\n"
		"    │░░░░░░░░░░█▀█░▀█▀░▀█▀░█▀█░░░░░░░░░░│\n"
		"    │░░░░░░░░░░█▀█░░█░░░█░░█▀█░░░░░░░░░░│\n"
		"    │░▀░░▀░░▀░░▀░▀░░▀░░░▀░░▀░▀░░▀░░▀░░▀░│\n"
		"    └───────────────────────────────────┘\n" <<
		"Welcome to Atta Robot Simulator! - by BrenoCQ\n\n"
		COLOR_RESET;
}
