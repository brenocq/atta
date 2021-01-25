//--------------------------------------------------
// Atta Physics
// dragForce.h
// Date: 2020-12-04
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_PHY_DRAG_FORCE_H
#define ATTA_PHY_DRAG_FORCE_H

#include "force.h"
#include "simulator/physics/body.h"

namespace atta::phy
{
	class DragForce : public Force
	{
		public:
			DragForce(float k1, float k2);
			~DragForce();

			virtual void updateForce(std::shared_ptr<Body> object, float dt);

		private:
			float _k1, _k2;
	};
}

#endif// ATTA_PHY_DRAG_FORCE_H
