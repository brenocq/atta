//--------------------------------------------------
// Atta Physics
// shape.h
// Date: 2020-12-10
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_PHY_SHAPE_H
#define ATTA_PHY_SHAPE_H

#include "simulator/math/math.h"

namespace atta::phy
{
	enum ShapeType
	{
		SPHERE_SHAPE = 0,
		BOX_SHAPE,
		HALF_SPACE_SHAPE,
		PLANE_SHAPE,
		CYLINDER_SHAPE,
		CAPSULE_SHAPE
	};

	class Body;
	class Shape
	{
		public:
			Shape(vec3 position = vec3(),
					quat orientation = quat(),
					vec3 scale = vec3());
			~Shape();

			//---------- Getters ----------//
			ShapeType getType() const { return _type; }
			Body* getBody() const { return _body; }
			vec3 getPosition() const { return _position; }
			quat getOrientation() const { return _orientation; }
			vec3 getScale() const { return _scale; }

			//---------- Setters ----------//
			void setBody(Body* body) { _body = body; } 

		protected:
			void setType(ShapeType type) { _type = type; } 

		private:
			ShapeType _type;
			Body* _body;

			// Position and orientation in objectPhysics local space
			vec3 _position;
			quat _orientation;
			vec3 _scale;
	};
}

#endif// ATTA_PHY_SHAPE_H
