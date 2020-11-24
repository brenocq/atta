//--------------------------------------------------
// Robot Simulator
// miniCleaner.h
// Date: 2020-09-01
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef MINI_CLEANER_H
#define MINI_CLEANER_H

#include <string>
#include <vector>
#include "simulator/object.h"

class MiniCleaner : Object
{
	public:
		MiniCleaner(glm::vec3 position={0,0,0}, glm::vec3 rotation={0,0,0});
		~MiniCleaner();

		void run(float dt);

	private:
};

#endif// MINI_CLEANER_H

