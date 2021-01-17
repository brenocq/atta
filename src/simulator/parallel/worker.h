//--------------------------------------------------
// Atta Parallel
// threadPool.h
// Date: 2021-01-07
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_WORKER_H
#define ATTA_WORKER_H

#include <vector>
#include <thread>

namespace atta
{
	class Worker
	{
		public:
			Worker();
			~Worker();

			virtual void operator()() = 0;

		private:
	};
}

#endif// ATTA_WORKER_H
