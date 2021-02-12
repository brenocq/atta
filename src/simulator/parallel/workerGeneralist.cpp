//--------------------------------------------------
// Atta Parallel
// workerGeneralist.cpp
// Date: 2021-01-07
// By Breno Cunha Queiroz
//--------------------------------------------------
#include "workerGeneralist.h"
#include <iostream>

namespace atta
{
	WorkerGeneralist::WorkerGeneralist(CreateInfo createInfo):
		_setupStageBarrier(createInfo.setupStageBarrier),
		_physicsStageBarrier(createInfo.physicsStageBarrier),
		_renderingStageBarrier(createInfo.renderingStageBarrier),
		_robotStageBarrier(createInfo.robotStageBarrier)
	{

	}

	WorkerGeneralist::~WorkerGeneralist()
	{

	}

	void WorkerGeneralist::operator()()
	{
		std::cout << "Setup\n";
		_setupStageBarrier->wait();

		while(true)
		{
			//std::cout << "Physics\n";
			_physicsStageBarrier->wait();
			//std::cout << "Robot\n";
			_robotStageBarrier->wait();
			//std::cout << "Rendering\n";
			_renderingStageBarrier->wait();
		}

		std::cout << "Genralist Stop\n";
	}
}
