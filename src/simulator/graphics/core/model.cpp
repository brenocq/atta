//--------------------------------------------------
// Atta Graphics
// model.cpp
// Date: 2021-01-08
// By Breno Cunha Queiroz
//--------------------------------------------------
#include "model.h"
#include "simulator/helpers/log.h"

namespace atta
{
	std::map<std::string, std::weak_ptr<Mesh>> Model::allMeshes = {};

	Model::Model(CreateInfo info):
		_meshName(info.meshName)
	{
		if(_meshName == "atta::empty")
		{
			Log::warning("Model", "Mesh name should not be empty");
			return;
		}

		//----- Load mesh if necessary -----//
		if(allMeshes.find(_meshName) != allMeshes.end())// Already loaded to memory
		{
			// Get loaded mesh from memory
			_mesh = allMeshes[_meshName].lock();
		}
		else// Need to load into memory
		{
			// Create mesh
			_mesh = std::make_shared<Mesh>(_meshName);
			// Save mesh for future reference if another object uses this mesh
			allMeshes[_meshName] = _mesh;
		}
	}

	Model::~Model()
	{

	}
}
