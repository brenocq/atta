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
	atta::HalfSpace::CreateInfo groundInfo {
		.name = "Ground",
		.normal = {0.0,1.0,0},
		.offset = 0,
		.material = {
			.albedo = {.1,.8,.9}
		}
	};
	objects.push_back(std::make_shared<atta::HalfSpace>(groundInfo));

	atta::Box::CreateInfo boxInfo;

	boxInfo.name = "Box";
	boxInfo.position = {-3,0.5,0};
	boxInfo.scale = {1,1,1};
	boxInfo.mass = 1.0f,
	boxInfo.material.albedo = {1.0,.1,.1};
	objects.push_back(std::make_shared<atta::Box>(boxInfo));

	boxInfo.name = "Box";
	boxInfo.position = {-1,0.5,0};
	boxInfo.scale = {1,1,1};
	boxInfo.mass = 1.0f,
	boxInfo.material.albedo = {1.0,.1,.1};
	objects.push_back(std::make_shared<atta::Box>(boxInfo));

	boxInfo.name = "Box";
	boxInfo.position = {1,0.5,0};
	boxInfo.scale = {1,1,1};
	boxInfo.mass = 1.0f,
	boxInfo.material.albedo = {1.0,.1,.1};
	objects.push_back(std::make_shared<atta::Box>(boxInfo));

	boxInfo.name = "Box";
	boxInfo.position = {3,0.5,0};
	boxInfo.scale = {1,1,1};
	boxInfo.mass = 1.0f,
	boxInfo.material.albedo = {1.0,.1,.1};
	objects.push_back(std::make_shared<atta::Box>(boxInfo));

	atta::ImportedObject::CreateInfo wheel {
		.name = "Wheel",
		.fileName = "wheel",
		.position = {0,1.2,0},
		.rotation = {0,0,0},
		.scale = {3,3,3},
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
