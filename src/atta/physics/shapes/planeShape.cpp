//--------------------------------------------------
// Atta Physics
// planeShape.h
// Date: 2021-01-26
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/physics/shapes/planeShape.h>

namespace atta::phy
{
	PlaneShape::PlaneShape(vec3 position, quat orientation, vec2 size):
		Shape(position, orientation, vec3(size.x, 1, size.y))
	{
		setType(PLANE_SHAPE);
		setTypeName("PLANE");
	}

	PlaneShape::~PlaneShape()
	{

	}
}
