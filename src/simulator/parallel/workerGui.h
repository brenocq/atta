//--------------------------------------------------
// Atta Parrallel
// workerGui.h
// Date: 2021-01-07
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_WORKER_GUI_H
#define ATTA_WORKER_GUI_H

#include "worker.h"
#include "simulator/graphics/core/window.h"

namespace atta
{
	class WorkerGui : public Worker
	{
		public:
			WorkerGui();
			~WorkerGui();

			void operator()();

		private:
			std::shared_ptr<Window> _window;
	};
}

#endif// ATTA_WORKER_GUI_H
