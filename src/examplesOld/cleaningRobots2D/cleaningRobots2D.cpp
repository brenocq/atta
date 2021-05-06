//--------------------------------------------------
// Atta Cleaning Robots 2D Example
// cleaningRobots2D.cpp
// Date: 2021-02-11
// By Breno Cunha Queiroz
//--------------------------------------------------
#include "cleaningRobots2D.h"
#include "simulator/objects/basics/basics.h"
#include "utils.h"

namespace example
{
	CleaningRobots2D::CleaningRobots2D():
		_qtyRobots(20)
	{
		// Setup simulator
		_attaCreateInfo.dimensionMode = atta::DIM_MODE_2D;
		_attaCreateInfo.physicsMode = atta::PHY_MODE_DISABLED;
		_attaCreateInfo.robotProcessing = atta::ROBOT_PROCESSING_SEQUENTIAL;
		_attaCreateInfo.createWindow = true;

		// Setup house objects (floor, walls, furniture...)
		std::vector<std::shared_ptr<atta::Object>> objects = house.getObjects();
		_attaCreateInfo.objects = objects;

		// Setup robots (cleaners)
		createRobots();
		_attaCreateInfo.robots = cleanersToRobots();

		// Setup global processing
		_attaCreateInfo.runAfterRobots = [this](){ runAfterRobots(); };
	}

	CleaningRobots2D::~CleaningRobots2D()
	{

	}

	//----------------------------------------------------------------------------//
	//----------------------------- Run after robots -----------------------------//
	//----------------------------------------------------------------------------//
	void CleaningRobots2D::runAfterRobots()
	{
		// Update texture floor from robot positions
		for(const auto& cleaner : _cleaners)
			robotCleanPosition(cleaner);
		house.writeFloorDataToTexture();// Update texture on the GPU

		// Change theta on collision with wall
		// TODO 2D physics + contact sensor
		for(const auto& cleaner : _cleaners)
		{
			atta::vec3 position = cleaner->getPosition();
			float radius = cleaner->getRadius();
			float maxPos = (house.getFloorSize()*.5-0.2-radius);///*halfSize*/-0.15/*wall*/-radius);
			if(position.x<-maxPos)
				cleaner->setTheta(M_PI);
			if(position.x>maxPos)
				cleaner->setTheta(0);
			if(position.z<-maxPos)
				cleaner->setTheta(M_PI*1.5);
			if(position.z>maxPos)
				cleaner->setTheta(M_PI*0.5);
		}
	}

	void CleaningRobots2D::robotCleanPosition(std::shared_ptr<Cleaner> cleaner)
	{
		atta::vec3 position = cleaner->getPosition();
		int texWidthHeight = house.getFloorSize()*100;
		int xc = (position.x*100)/*cm*/ + texWidthHeight*0.5f;// Floor data robot x (centimeters from the left)
		int yc = (position.z*100)/*cm*/ + texWidthHeight*0.5f;// Floor data robot y (centimeters from the top)
		int radius = cleaner->getRadius()*100;
		float theta = cleaner->getTheta();

		cleanSolidCircle(house.floorData, texWidthHeight, xc, yc, radius);
	}

	//----------------------------------------------------------------------------//
	//------------------------------ Robot creation ------------------------------//
	//----------------------------------------------------------------------------//
	void CleaningRobots2D::createRobots()
	{
		// Create robots
		for(unsigned i=0; i<_qtyRobots; i++)
		{
			float x = rand()%1000/1000.f*6-3;
			float z = rand()%1000/1000.f*6-3;
			auto cleaner = std::make_shared<Cleaner>(atta::vec3(x, .5, z));
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
}
