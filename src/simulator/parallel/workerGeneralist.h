//--------------------------------------------------
// Atta Parallel
// workerGeneralist.h
// Date: 2021-01-07
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_WORKER_GENERALIST_H
#define ATTA_WORKER_GENERALIST_H

#include "worker.h"
#include "barrier.h"

namespace atta
{
	class WorkerGeneralist : public Worker
	{
		public:
			WorkerGeneralist(std::shared_ptr<Barrier> barrier);
			~WorkerGeneralist();

			void operator()();

		private:
			std::shared_ptr<Barrier> _barrier;
	};
}

#endif// ATTA_WORKER_GENERALIST_H
