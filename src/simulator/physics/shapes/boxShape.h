//--------------------------------------------------
// Atta Physics
// boxShape.h
// Date: 2020-12-10
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_PHY_BOX_SHAPE_H
#define ATTA_PHY_BOX_SHAPE_H

#include "shape.h"

namespace atta::phy
{
	class BoxShape : public Shape
	{
		public:
			BoxShape(vec3 position = vec3(), quat orientation = quat(), vec3 size = vec3(1,1,1));
			~BoxShape();

		private:
	};
}

#endif// ATTA_PHY_BOX_SHAPE_H
