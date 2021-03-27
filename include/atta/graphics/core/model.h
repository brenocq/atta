//--------------------------------------------------
// Atta Graphics
// model.h
// Date: 2021-01-08
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_GRAPHICS_CORE_MODEL_H
#define ATTA_GRAPHICS_CORE_MODEL_H

#include <string>
#include <map>
#include <memory>
#include <atta/graphics/core/mesh.h>
#include <atta/graphics/core/material.h>

namespace atta
{
	class Model
	{
		public:
			struct CreateInfo
			{
				std::string meshName = "atta::empty";// Can be the filename or "atta::{meshType}"
				Material material = {};
				std::map<std::string, Material> materials = {};
			};

			Model(CreateInfo info);
			~Model();

			// Store all loaded meshes to avoid duplicate meshes in memory
			// map<fileName, Mesh*>
			static std::map<std::string, std::weak_ptr<Mesh>> allMeshes;

			//---------- Setters ----------//
			void setMaterialOffset(int materialOffset) { _materialOffset = materialOffset; }

			//---------- Getters ----------//
			std::map<std::string, Material> getMaterials() const { return _materials; }
			int getMaterialOffset() const { return _materialOffset; }
			std::string getMeshName() const { return _meshName; }
			std::shared_ptr<Mesh> getMesh() const { return _mesh; }
			unsigned getMeshIndex() const { return _mesh->getIndex(); }

		private:
			std::string _meshName;
			std::shared_ptr<Mesh> _mesh;
			std::map<std::string, Material> _materials;
			int _materialOffset;
	};
}

#endif// ATTA_GRAPHICS_CORE_MODEL_H
