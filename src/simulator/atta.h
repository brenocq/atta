//--------------------------------------------------
// Atta Robot Simulator
// atta.h
// Date: 2020-06-21
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_SIMULATOR_H
#define ATTA_SIMULATOR_H

#include <string>
#include <vector>
#include <memory>
#include "simulator/parallel/threadManager.h"
#include "simulator/objects/object.h"
//#include "simulator/helpers/drawer.h"

namespace atta
{
	class Atta
	{
		public:
			struct CreateInfo
			{
				std::vector<std::shared_ptr<Object>> objects;
			};

			Atta(CreateInfo createInfo);
			~Atta();

			void run();

		private:
			std::shared_ptr<ThreadManager> _threadManager;
	};
}

#endif// ATTA_SIMULATOR_H
