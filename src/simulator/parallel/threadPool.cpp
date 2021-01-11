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
		_qtyWorkersToCreate = qtyThreads==-1 ? maxSystemCores-1 : qtyThreads;

		Log::info("ThreadPool", "Detected $0 cores, $1 workers can be created", maxSystemCores, _qtyWorkersToCreate);

		//                                                             ,-------------------.
		//                                                             v                   |
		// Barrier to syncronize generalist workers state (start -> physics -> render -> robots -> end)
		_generalistStateBarrier = std::make_shared<Barrier>(_qtyWorkersToCreate-1);
	}

	ThreadPool::~ThreadPool()
	{
		for(auto& t : _threads)
			t.join();
	}


	void ThreadPool::createGeneralistWorkers()
	{
		// Create generalists
		for(unsigned i = 0; i < _qtyWorkersToCreate-1; ++i)
		{
			_workersGen.push_back(std::make_shared<WorkerGeneralist>(_generalistStateBarrier));
			_threads.push_back(std::thread(*_workersGen[i]));
		}
		Log::success("ThreadPool", "Created $0 generalist workers.", _qtyWorkersToCreate-1);
	}

	void ThreadPool::createGuiWorker()
	{
		_workerGui = std::make_shared<WorkerGui>();
		_threads.push_back(std::thread(*_workerGui));
		Log::success("ThreadPool", "Created 1 GUI worker.");
	}
}
