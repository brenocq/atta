//--------------------------------------------------
// Robot Simulator
// importedObject.cpp
// Date: 2020-08-18
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/objects/basics/importedObject.h>

namespace atta
{
	ImportedObject::ImportedObject(CreateInfo info):
		Object({info.name, info.position, info.rotation, info.scale, info.mass})
	{
		Object::setType("ImportedObject");

		//----- Create model -----//
		Model::CreateInfo modelInfo = {
			.meshName = info.fileName,
			.material = info.material
		};

		_model = std::make_shared<Model>(modelInfo);

		//----- Physics -----//
		_bodyPhysics->addShape(std::make_shared<phy::BoxShape>(vec3(), quat(), vec3(1,1,1)));
	}

	ImportedObject::~ImportedObject()
	{
	}
}
