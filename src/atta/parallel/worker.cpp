//--------------------------------------------------
// Atta Parallel
// worker.cpp
// Date: 2021-01-07
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/parallel/worker.h>
#include <iostream>

namespace atta
{
	Worker::Worker():
		_shouldFinish(false)
	{
	}

	Worker::~Worker()
	{
	}
}
