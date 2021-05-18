//--------------------------------------------------
// Robot Simulator
// box.cpp
// Date: 2020-08-30
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/objects/basics/box.h>
#include <atta/physics/shapes/boxShape.h>

namespace atta
{
	Box::Box(CreateInfo info):
		Object({info.name, info.position, info.rotation, info.scale, info.mass, std::move(info.children)})
	{
		Object::setType("Box");

		//----- Create model -----//
		Model::CreateInfo modelInfo = {
			.meshName = "atta::box",
			.material = info.material
		};
		_model = std::make_shared<Model>(modelInfo);

		//----- Physics -----//
		_bodyPhysics->addShape(std::make_shared<phy::BoxShape>(vec3(), quat(), vec3(1,1,1)));
	}

	Box::~Box()
	{
	}
}
