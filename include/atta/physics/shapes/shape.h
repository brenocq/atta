//--------------------------------------------------
// Atta Physics
// shape.h
// Date: 2020-12-10
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_PHYSICS_SHAPES_SHAPE_H
#define ATTA_PHYSICS_SHAPES_SHAPE_H

#include <atta/math/math.h>

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

	enum ShapePairType
	{
		SPHERE_SPHERE_PAIR = 			100*SPHERE_SHAPE+SPHERE_SHAPE,
		SPHERE_BOX_PAIR = 				100*SPHERE_SHAPE+BOX_SHAPE,
		SPHERE_HALF_SPACE_PAIR = 		100*SPHERE_SHAPE+HALF_SPACE_SHAPE,
		SPHERE_PLANE_PAIR = 			100*SPHERE_SHAPE+PLANE_SHAPE,
		SPHERE_CYLINDER_PAIR = 			100*SPHERE_SHAPE+CYLINDER_SHAPE,
		SPHERE_CAPSULE_PAIR = 			100*SPHERE_SHAPE+CAPSULE_SHAPE,

		BOX_SPHERE_PAIR = 				100*BOX_SHAPE+SPHERE_SHAPE,
		BOX_BOX_PAIR = 					100*BOX_SHAPE+BOX_SHAPE,
		BOX_HALF_SPACE_PAIR = 			100*BOX_SHAPE+HALF_SPACE_SHAPE,
		BOX_PLANE_PAIR = 				100*BOX_SHAPE+PLANE_SHAPE,
		BOX_CYLINDER_PAIR = 			100*BOX_SHAPE+CYLINDER_SHAPE,
		BOX_CAPSULE_PAIR = 				100*BOX_SHAPE+CAPSULE_SHAPE,

		HALF_SPACE_SPHERE_PAIR = 		100*HALF_SPACE_SHAPE+SPHERE_SHAPE,
		HALF_SPACE_BOX_PAIR = 			100*HALF_SPACE_SHAPE+BOX_SHAPE,
		HALF_SPACE_HALF_SPACE_PAIR = 	100*HALF_SPACE_SHAPE+HALF_SPACE_SHAPE,
		HALF_SPACE_PLANE_PAIR = 		100*HALF_SPACE_SHAPE+PLANE_SHAPE,
		HALF_SPACE_CYLINDER_PAIR = 		100*HALF_SPACE_SHAPE+CYLINDER_SHAPE,
		HALF_SPACE_CAPSULE_PAIR = 		100*HALF_SPACE_SHAPE+CAPSULE_SHAPE,

		PLANE_SPHERE_PAIR = 			100*PLANE_SHAPE+SPHERE_SHAPE,
		PLANE_BOX_PAIR = 				100*PLANE_SHAPE+BOX_SHAPE,
		PLANE_HALF_SPACE_PAIR = 		100*PLANE_SHAPE+HALF_SPACE_SHAPE,
		PLANE_PLANE_PAIR = 				100*PLANE_SHAPE+PLANE_SHAPE,
		PLANE_CYLINDER_PAIR = 			100*PLANE_SHAPE+CYLINDER_SHAPE,
		PLANE_CAPSULE_PAIR = 			100*PLANE_SHAPE+CAPSULE_SHAPE,

		CYLINDER_SPHERE_PAIR = 			100*CYLINDER_SHAPE+SPHERE_SHAPE,
		CYLINDER_BOX_PAIR = 			100*CYLINDER_SHAPE+BOX_SHAPE,
		CYLINDER_HALF_SPACE_PAIR = 		100*CYLINDER_SHAPE+HALF_SPACE_SHAPE,
		CYLINDER_PLANE_PAIR = 			100*CYLINDER_SHAPE+PLANE_SHAPE,
		CYLINDER_CYLINDER_PAIR = 		100*CYLINDER_SHAPE+CYLINDER_SHAPE,
		CYLINDER_CAPSULE_PAIR = 		100*CYLINDER_SHAPE+CAPSULE_SHAPE,

		CAPSULE_SPHERE_PAIR = 			100*CAPSULE_SHAPE+SPHERE_SHAPE,
		CAPSULE_BOX_PAIR = 				100*CAPSULE_SHAPE+BOX_SHAPE,
		CAPSULE_HALF_SPACE_PAIR = 		100*CAPSULE_SHAPE+HALF_SPACE_SHAPE,
		CAPSULE_PLANE_PAIR = 			100*CAPSULE_SHAPE+PLANE_SHAPE,
		CAPSULE_CYLINDER_PAIR = 		100*CAPSULE_SHAPE+CYLINDER_SHAPE,
		CAPSULE_CAPSULE_PAIR = 			100*CAPSULE_SHAPE+CAPSULE_SHAPE
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
			std::string getTypeName() const { return _typeName; }
			Body* getBody() const { return _body; }
			vec3 getPosition() const { return _position; }
			quat getOrientation() const { return _orientation; }
			vec3 getScale() const { return _scale; }
			virtual mat3 calculateInertiaTensor(float mass) = 0;

			//---------- Setters ----------//
			void setBody(Body* body) { _body = body; } 

		protected:
			void setType(ShapeType type) { _type = type; } 
			void setTypeName(std::string typeName) { _typeName = typeName; } 

			ShapeType _type;
			std::string _typeName;
			Body* _body;

			// Position and orientation in objectPhysics local space
			vec3 _position;
			quat _orientation;
			vec3 _scale;
	};
}

#endif// ATTA_PHYSICS_SHAPES_SHAPE_H
