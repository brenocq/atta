//--------------------------------------------------
// Atta Graphics
// model.h
// Date: 2021-01-08
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_MODEL_H
#define ATTA_MODEL_H

#include <string>
#include <map>
#include <memory>
#include "simulator/graphics/core/mesh.h"
#include "simulator/graphics/core/material.h"

namespace atta
{
	class Model
	{
		public:
			struct CreateInfo
			{
				std::string meshName = "atta::empty";// Can be the filename or "atta::{meshType}"
			};

			Model(CreateInfo info);
			~Model();

		private:
			// Store all loaded meshes to avoid duplicate meshes in memory
			// map<fileName, Mesh*>
			static std::map<std::string, std::weak_ptr<Mesh>> allMeshes;

			std::string _meshName;
			std::shared_ptr<Mesh> _mesh;
			std::shared_ptr<Material> _material;
	};
}

#endif// ATTA_MODEL_H
