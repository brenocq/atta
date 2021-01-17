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
	WorkerGeneralist::WorkerGeneralist(std::shared_ptr<Barrier> barrier):
		_barrier(barrier)
	{

	}

	WorkerGeneralist::~WorkerGeneralist()
	{

	}

	void WorkerGeneralist::operator()()
	{
		//std::cout << "Generalist worker\n";
		_barrier->wait();
		//std::cout << "Generalist start\n";
		_barrier->wait();
		//std::cout << "Physics\n";
		_barrier->wait();
		//std::cout << "Render\n";
		_barrier->wait();
		//std::cout << "Robot\n";
		return;
		//std::cout << "Genralist Stop\n";
	}
}
