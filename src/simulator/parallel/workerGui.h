//--------------------------------------------------
// Atta Parrallel
// workerGui.h
// Date: 2021-01-07
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_WORKER_GUI_H
#define ATTA_WORKER_GUI_H

#include "worker.h"

namespace atta
{
	class WorkerGui : public Worker
	{
		public:
			WorkerGui();
			~WorkerGui();

			void operator()();

		private:
	};
}

#endif// ATTA_WORKER_GUI_H
