//--------------------------------------------------
// Atta Parrallel
// threadPool.h
// Date: 2021-01-07
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_THREAD_POOL_H
#define ATTA_THREAD_POOL_H

#include <vector>
#include <thread>
#include <memory>
#include "barrier.h"
#include "workerGeneralist.h"
#include "workerGui.h"

namespace atta
{
	class ThreadPool
	{
		public:
			ThreadPool(int qtyThreads = -1);
			~ThreadPool();

		private:

			// Syncronization structures
			std::shared_ptr<Barrier> _generalistStateBarrier;

			// Threads
			std::vector<std::thread> _threads;
			// Workers
			std::vector<std::shared_ptr<WorkerGeneralist>> _workersGen;
			std::shared_ptr<WorkerGui> _workerGui;
	};
}

#endif// ATTA_THREAD_POOL_H
