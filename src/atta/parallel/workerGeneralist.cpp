//--------------------------------------------------
// Atta Parallel
// workerGeneralist.cpp
// Date: 2021-01-07
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/parallel/workerGeneralist.h>
#include <iostream>
#include <atta/helpers/log.h>

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
		//Log::info("WorkerGeneralist", "Destroyed");
	}

	void WorkerGeneralist::operator()()
	{
		//std::cout << "Setup\n";
		_setupStageBarrier->wait();

		while(!_shouldFinish)
		{
			//std::cout << "Physics\n";
			_physicsStageBarrier->wait();
			//std::cout << "Robot\n";
			_robotStageBarrier->wait();
			//std::cout << "Rendering\n";
			_renderingStageBarrier->wait();
			//std::cout << "Evaluate\n";
		}
		//std::cout << "Finished\n";
	}
}
