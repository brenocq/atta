//--------------------------------------------------
// Atta Physics
// halfSpaceShape.h
// Date: 2021-01-26
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_PHY_HALF_SPACE_SHAPE_H
#define ATTA_PHY_HALF_SPACE_SHAPE_H

#include "shape.h"

namespace atta::phy
{
	class HalfSpaceShape : public Shape
	{
		public:
			HalfSpaceShape(vec3 normal, float offset);
			~HalfSpaceShape();

			//---------- Getters ----------//
			vec3 getNormal() const { return _normal; }
			float getOffset() const { return _offset; }

		private:
			vec3 _normal;
			float _offset;
	};
}

#endif// ATTA_PHY_HALF_SPACE_SHAPE_H
