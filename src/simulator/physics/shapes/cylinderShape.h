//--------------------------------------------------
// Atta Physics
// cylinderShape.h
// Date: 2020-12-10
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_PHY_CYLINDER_SHAPE_H
#define ATTA_PHY_CYLINDER_SHAPE_H

#include "shape.h"

namespace atta::phy
{
	class CylinderShape : public Shape
	{
		public:
			CylinderShape(vec3 position = vec3(), quat orientation = quat(), vec3 size = vec3(1,1,1));
			~CylinderShape();

		private:
	};
}

#endif// ATTA_PHY_CYLINDER_SHAPE_H
