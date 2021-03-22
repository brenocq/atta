//--------------------------------------------------
// Robot Simulator
// cylinder.cpp
// Date: 2020-08-30
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/objects/basics/cylinder.h>
#include <atta/physics/shapes/cylinderShape.h>

namespace atta
{
	Cylinder::Cylinder(CreateInfo info):
		Object({info.name, info.position, info.rotation, vec3(info.radius*2, info.height, info.radius*2), info.mass})
	{
		Object::setType("Cylinder");

		//----- Create model -----//
		Model::CreateInfo modelInfo = {
			.meshName = "atta::cylinder",
			.material = info.material
		};

		_model = std::make_shared<Model>(modelInfo);

		//----- Physics -----//
		_bodyPhysics->addShape(std::make_shared<phy::CylinderShape>(vec3(), quat(), _scale));
	}

	Cylinder::~Cylinder()
	{
	}
}
