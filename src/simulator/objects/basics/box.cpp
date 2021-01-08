//--------------------------------------------------
// Robot Simulator
// box.cpp
// Date: 2020-08-30
// By Breno Cunha Queiroz
//--------------------------------------------------
#include "box.h"

namespace atta
{
	Box::Box(CreateInfo info):
		Object({info.name, info.position, info.rotation, info.scale, info.mass}), _color(info.color)
	{
		Object::setType("Box");

		//----- Create model -----//
		Model::CreateInfo modelInfo = {
			.meshName = "atta::box"
		};

		std::shared_ptr<Model> model = std::make_shared<Model>(modelInfo);
		Object::setModel(model);
	}

	Box::~Box()
	{
	}
}
