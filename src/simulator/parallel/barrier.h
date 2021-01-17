//--------------------------------------------------
// Atta Parallel
// barrier.h
// Date: 2021-01-07
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_BARRIER_H
#define ATTA_BARRIER_H

#include <mutex>
#include <condition_variable>
#include <atomic>

namespace atta
{
	class Barrier
	{
		public:
			Barrier(int qtyThreads);
			~Barrier();

			void wait();

		private:
			unsigned _qtyThreads;// Total qty threads
			std::mutex _mutex;
			std::condition_variable _cv;
			unsigned _count;// Quantity left to reach the barrier
			bool _up;// Count up or down the left quantity
	};
}

#endif// ATTA_BARRIER_H
