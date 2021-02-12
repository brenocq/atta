//--------------------------------------------------
// Atta Cleaning Robots 2D Example
// cleaningRobots2D.cpp
// Date: 2021-02-11
// By Breno Cunha Queiroz
//--------------------------------------------------
#include "cleaningRobots2D.h"
#include "simulator/objects/basics/basics.h"

CleaningRobots2D::CleaningRobots2D():
	_qtyRobots(9)
{
	_attaCreateInfo.dimensionMode = atta::Atta::DIM_MODE_2D;
	_attaCreateInfo.physicsMode = atta::Atta::PHY_MODE_DISABLED;
	_attaCreateInfo.createWindow = true;

	// House objects (floor, walls, furniture...)
	std::vector<std::shared_ptr<atta::Object>> objects = house.getObjects();
	_attaCreateInfo.objects = objects;

	createRobots();
	_attaCreateInfo.robots = cleanersToRobots();
}

CleaningRobots2D::~CleaningRobots2D()
{

}


void CleaningRobots2D::createRobots()
{
	// Create robots
	for(unsigned i=0; i<_qtyRobots; i++)
	{
		auto cleaner = std::make_shared<Cleaner>(atta::vec3(i%3, 0, i/3));
		_cleaners.push_back(cleaner);
	}
}

std::vector<std::shared_ptr<atta::Robot>> CleaningRobots2D::cleanersToRobots()
{
	std::vector<std::shared_ptr<atta::Robot>> robots;

	for(auto cleaner : _cleaners)
		robots.push_back(std::static_pointer_cast<atta::Robot>(cleaner));

	return robots;
}
