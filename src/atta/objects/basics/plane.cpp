//--------------------------------------------------
// Robot Simulator
// plane.cpp
// Date: 2020-08-18
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/objects/basics/plane.h>
#include <atta/physics/shapes/planeShape.h>

namespace atta
{
	Plane::Plane(CreateInfo info):
		Object({info.name, info.position, info.rotation, {info.size.x,1,info.size.y}, info.mass})
	{
		Object::setType("Plane");

		//----- Create model -----//
		Model::CreateInfo modelInfo = {
			.meshName = "atta::plane",
			.material = info.material
		};

		_model = std::make_shared<Model>(modelInfo);

		//----- Physics -----//
		_bodyPhysics->addShape(std::make_shared<phy::PlaneShape>(vec3(), quat(), info.size));
	}

	Plane::~Plane()
	{
	}
}
