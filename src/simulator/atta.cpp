//--------------------------------------------------
// Atta Robot Simulator
// atta.cpp
// Date: 2020-06-21
// By Breno Cunha Queiroz
//--------------------------------------------------
#include "atta.h"

#include "simulator/helpers/log.h"
//#include "simulator/helpers/evaluator.h"

namespace atta
{
	Atta::Atta(CreateInfo createInfo)
	{
		// Create thread pool
		_threadPool = std::make_shared<ThreadPool>();

		// Create window
		_window = std::make_shared<Window>();

		// Create scene
		_scene = std::make_shared<Scene>({.objects = createInfo.objects});

		// Initialize physics
		
		// Initialize renderer
		_renderer = std::make_shared<Renderer>(_window, _scene);

		// Initialize objects

		// Start simulator
	}

	Atta::~Atta()
	{
	}

	void Atta::run()
	{
	}
}
