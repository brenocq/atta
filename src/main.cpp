#include <iostream>
#include "simulator/atta.h"
#include "simulator/objects/basics/basics.h"
#include "simulator/helpers/log.h"

void welcome();

int main()
{
	welcome();
	//---------- Create Scene ----------//
	std::vector<std::shared_ptr<atta::Object>> objects = {};
	atta::Box::CreateInfo boxInfo {
		.name = "Box",
		.position = {0,0,0},
		.rotation = {0,0,0},
		.scale = {1,1,1},
		.mass = 1.0f,
		.material = {
			.color = {1,0,0}
		}
	};
	objects.push_back(std::make_shared<atta::Box>(boxInfo));

	atta::ImportedObject::CreateInfo wheel {
		.name = "Wheel",
		.fileName = "wheel",
		.position = {0,0,0},
		.rotation = {0,0,0},
		.scale = {1,1,1},
		.mass = 1.0f
	};
	objects.push_back(std::make_shared<atta::ImportedObject>(wheel));

	//---------- Create Atta Simulator ----------//
	atta::Atta::CreateInfo attaCreateInfo = {
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
