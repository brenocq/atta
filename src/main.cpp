#include <iostream>
#include "simulator/atta.h"
#include "simulator/objects/basics/basics.h"
#include "simulator/helpers/log.h"

void welcome();

int main()
{
	welcome();

	std::vector<std::shared_ptr<atta::Object>> objects = {};
	atta::HalfSpace::CreateInfo groundInfo {
		.name = "Ground",
		.normal = {0,1,0},
		.offset = 0,
		.material = {
			.albedo = {.4,.4,.4}
		}
	};
	objects.push_back(std::make_shared<atta::HalfSpace>(groundInfo));

	atta::Box::CreateInfo boxInfo {
		.name = "Box",
		.position = {0,0.5,0},
		.rotation = {0,0,0},
		.scale = {1,1,1},
		.material = {
			.albedo = {.6,.1,.1}
		}
	};
	objects.push_back(std::make_shared<atta::Box>(boxInfo));

	boxInfo = {
		.name = "Box",
		.position = {6.16666,0.5,4.5},
		.rotation = {0,0,0},
		.scale = {1,1,1},
		.material = {
			.albedo = {.1,.1,.6}
		}
	};
	objects.push_back(std::make_shared<atta::Box>(boxInfo));


	//---------- Create Atta Simulator ----------//
	atta::Atta::CreateInfo attaCreateInfo = {
		.dimensionMode = atta::Atta::DIM_MODE_2D,
		.physicsMode = atta::Atta::PHY_MODE_DISABLED,
		.objects = objects
	};

	atta::Atta sim = atta::Atta(attaCreateInfo);
	sim.run();

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
