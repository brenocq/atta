//--------------------------------------------------
// Robot Simulator
// dragForce.h
// Date: 2020-12-04
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef DRAG_FORCE_H
#define DRAG_FORCE_H

#include "glm.h"
#include "force.h"
#include "../objectPhysics.h"

class DragForce : public Force
{
	public:
		DragForce(float k1, float k2);
		~DragForce();

		virtual void updateForce(ObjectPhysics* object, float dt);

	private:
		float _k1, _k2;
};

#endif// DRAG_FORCE_H
