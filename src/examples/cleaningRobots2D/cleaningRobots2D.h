//--------------------------------------------------
// Atta Cleaning Robots 2D Example
// cleaningRobots2D.h
// Date: 2021-02-11
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_EXAMPLE_CLEANING_ROBOTS_2D
#define ATTA_EXAMPLE_CLEANING_ROBOTS_2D
#include "simulator/project.h"
#include "house.h"
#include "cleaner.h"

class CleaningRobots2D : public atta::Project
{
	public:
		CleaningRobots2D();
		~CleaningRobots2D();

	private:
		void createRobots();
		std::vector<std::shared_ptr<atta::Robot>> cleanersToRobots();

		House house;
		std::vector<std::shared_ptr<Cleaner>> _cleaners;

		// Simulation parameters
		unsigned _qtyRobots;
};
#endif// ATTA_EXAMPLE_CLEANING_ROBOTS_2D
