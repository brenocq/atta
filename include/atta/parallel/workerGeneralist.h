//--------------------------------------------------
// Atta Parallel
// workerGeneralist.h
// Date: 2021-01-07
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_PARALLEL_WORKER_GENERALIST_H
#define ATTA_PARALLEL_WORKER_GENERALIST_H

#include <atta/parallel/worker.h>
#include <atta/parallel/barrier.h>

namespace atta
{
	class WorkerGeneralist : public Worker
	{
		public:
			struct CreateInfo
			{
				std::shared_ptr<Barrier> setupStageBarrier;
				std::shared_ptr<Barrier> physicsStageBarrier;
				std::shared_ptr<Barrier> sensorStageBarrier;
				std::shared_ptr<Barrier> robotStageBarrier;
			};
			WorkerGeneralist(CreateInfo createInfo);
			~WorkerGeneralist();

			void operator()();

		private:
			std::shared_ptr<Barrier> _setupStageBarrier;
			std::shared_ptr<Barrier> _physicsStageBarrier;
			std::shared_ptr<Barrier> _sensorStageBarrier;
			std::shared_ptr<Barrier> _robotStageBarrier;
	};
}

#endif// ATTA_PARALLEL_WORKER_GENERALIST_H
