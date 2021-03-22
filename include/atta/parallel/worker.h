//--------------------------------------------------
// Atta Parallel
// threadPool.h
// Date: 2021-01-07
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_PARALLEL_WORKER_H
#define ATTA_PARALLEL_WORKER_H

#include <vector>
#include <thread>
#include <atomic>

namespace atta
{
	class Worker
	{
		public:
			Worker();
			~Worker();

			virtual void operator()() = 0;

			void setShouldFinish(bool shouldFinish) { _shouldFinish=shouldFinish; }
			bool getShouldFinish() { return _shouldFinish; }
		protected:
			std::atomic<bool> _shouldFinish;
	};
}

#endif// ATTA_PARALLEL_WORKER_H
