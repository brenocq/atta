//--------------------------------------------------
// Robot Simulator
// importedObject.cpp
// Date: 2020-08-18
// By Breno Cunha Queiroz
//--------------------------------------------------
#include "importedObject.h"

namespace atta
{
	ImportedObject::ImportedObject(CreateInfo info):
		Object({info.name, info.position, info.rotation, info.scale, info.mass})
	{
		Object::setType("ImportedObject");

		//----- Create model -----//
		Model::CreateInfo modelInfo = {
			.meshName = info.fileName
		};
		std::shared_ptr<Model> model = std::make_shared<Model>(modelInfo);
		Object::setModel(model);
	}

	ImportedObject::~ImportedObject()
	{
	}
}
