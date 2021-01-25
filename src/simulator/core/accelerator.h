//--------------------------------------------------
// Robot Simulator
// accelerator.h
// Date: 2021-01-11
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_ACCELERATOR_H
#define ATTA_ACCELERATOR_H

#include <vector>
#include <memory>
#include "simulator/objects/object.h"

namespace atta
{
	class Accelerator
	{
		public:
			struct CreateInfo
			{
				std::vector<std::shared_ptr<Object>> objects;
			};

			Accelerator(CreateInfo info);
			~Accelerator();

			//---------- Getters ----------//
			std::vector<std::shared_ptr<Object>> getObjects() const { return _objects; }

		private:
			std::vector<std::shared_ptr<Object>> _objects;
	};
}
#endif// ATTA_ACCELERATOR_H
