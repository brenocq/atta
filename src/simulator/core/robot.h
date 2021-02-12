//--------------------------------------------------
// Atta Robot Simulator
// robot.h
// Date: 2021-02-11
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_ROBOT_H
#define ATTA_ROBOT_H

#include <vector>
#include "simulator/objects/object.h"

namespace atta
{
	class Robot
	{
		public:
			Robot();
			~Robot();

			virtual void run(float dt) = 0;

			//---------- Getters ----------//
			std::shared_ptr<Object> getRootObject() const { return _rootObject; }

		protected:
			std::shared_ptr<Object> _rootObject;
	};
}

#endif// ATTA_ROBOT_H
