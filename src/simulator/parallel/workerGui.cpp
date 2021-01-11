//--------------------------------------------------
// Atta Parrallel
// workerGui.cpp
// Date: 2021-01-07
// By Breno Cunha Queiroz
//--------------------------------------------------
#include "workerGui.h"
#include <iostream>

namespace atta
{
	WorkerGui::WorkerGui()
	{
		// Create window (GUI thread only)
		_window = std::make_shared<Window>();
	}

	WorkerGui::~WorkerGui()
	{
		std::cout << "GUI worker destroyed\n";
	}

	void WorkerGui::operator()()
	{
		// TODO check _window->shouldClose();
		while(true)
		{
			_window->poolEvents();
		}
	}
}
