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

	}

	WorkerGui::~WorkerGui()
	{

	}

	void WorkerGui::operator()()
	{
		std::cout << "GUI worker\n";
		while(true)
		{

		}
		std::cout << "GUI worker stop\n";
	}
}
