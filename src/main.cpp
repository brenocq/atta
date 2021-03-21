#include <iostream>
#include "examples/examples.h"
#include "simulator/atta.h"

void welcome();

int main()
{
	welcome();
	example::CleaningRobots2D cleaningRobots2D;
	atta::Atta simulator = atta::Atta::createFromProject(cleaningRobots2D);
	
	//example::MaterialsDemo materialsDemo;
	//atta::Atta simulator = atta::Atta::createFromProject(materialsDemo);
	
	//example::PhysicsDemo physicsDemo;
	//atta::Atta simulator = atta::Atta::createFromProject(physicsDemo);
	simulator.run();

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
