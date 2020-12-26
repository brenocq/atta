//--------------------------------------------------
// Atta Physics
// capsuleShape.h
// Date: 2020-12-10
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_PHY_CAPSULE_SHAPE_H
#define ATTA_PHY_CAPSULE_SHAPE_H

#include "shape.h"

namespace atta::phy
{
	class CapsuleShape : public Shape
	{
		public:
			CapsuleShape(vec3 position = vec3(), quat orientation = quat(), vec3 size = vec3(1,1,1));
			~CapsuleShape();

		private:
	};
}

#endif// ATTA_PHY_CAPSULE_SHAPE_H
