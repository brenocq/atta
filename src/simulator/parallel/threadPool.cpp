//--------------------------------------------------
// Atta Parrallel
// threadPool.cpp
// Date: 2021-01-07
// By Breno Cunha Queiroz
//--------------------------------------------------
#include "threadPool.h"
#include "simulator/helpers/log.h"

namespace atta
{
	ThreadPool::ThreadPool(int qtyThreads)
	{
		//----- Calculate quantity of workers -----//
		unsigned maxSystemCores = std::max(1u, std::thread::hardware_concurrency());
		// maxSystemCores-1 because main thread helps out, too
		unsigned qtyThreadsToCreate = qtyThreads==-1 ? maxSystemCores-1 : qtyThreads;

		Log::info("ThreadPool", "Detected $0 cores, creating $1 workers...", maxSystemCores, qtyThreadsToCreate);

		//                                                             ,-------------------.
		//                                                             v                   |
		// Barrier to syncronize generalist workers state (start -> physics -> render -> robots -> end)
		_generalistStateBarrier = std::make_shared<Barrier>(qtyThreadsToCreate-1);

		//----- Create workers -----//
		// Create generalists
		for(unsigned i = 0; i < qtyThreadsToCreate-1; ++i)
		{
			_workersGen.push_back(std::make_shared<WorkerGeneralist>(_generalistStateBarrier));
			_threads.push_back(std::thread(*_workersGen[i]));
		}

		// Create gui worker
		_workerGui = std::make_shared<WorkerGui>();
		_threads.push_back(std::thread(*_workerGui));
	}

	ThreadPool::~ThreadPool()
	{
		for(auto& t : _threads)
			t.join();
	}
}
