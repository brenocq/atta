//--------------------------------------------------
// Robot Simulator
// halfSpace.cpp
// Date: 2021-01-26
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/objects/basics/halfSpace.h>
#include <atta/physics/shapes/halfSpaceShape.h>

namespace atta
{
	HalfSpace::HalfSpace(CreateInfo info):
		Object({info.name, vec3(0,0,0), vec3(0,0,0), vec3(1,1,1), 0.0f})
	{
		Object::setType("HalfSpace");

		vec3 normal = normalize(info.normal);

		//----- Define position/orientation/scale from halfspace -----//
		_scale = vec3(10000, 1, 10000);
		_position = normal*info.offset;
		// TODO calculate change in orientation

		//----- Create model -----//
		Model::CreateInfo modelInfo = {
			.meshName = "atta::plane",
			.material = info.material
		};

		_model = std::make_shared<Model>(modelInfo);
		_bodyPhysics->addShape(std::make_shared<phy::HalfSpaceShape>(normal, info.offset));
	}

	HalfSpace::~HalfSpace()
	{
	}
}
