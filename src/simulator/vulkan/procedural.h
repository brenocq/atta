//--------------------------------------------------
// Robot Simulator
// procedural.h
// Date: 24/06/2020
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef PROCEDURAL_H
#define PROCEDURAL_H

#include <iostream>
#include <string.h>
#include "defines.h"
#include "glm.h"

class Procedural
{
	public:
	Procedural() = default;
	virtual ~Procedural() = default;;
	virtual std::pair<glm::vec3, glm::vec3> boundingBox() const = 0;
};

#endif// PROCEDURAL_H
