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
				Material material = {};
			};

			Model(CreateInfo info);
			~Model();

			// Store all loaded meshes to avoid duplicate meshes in memory
			// map<fileName, Mesh*>
			static std::map<std::string, std::weak_ptr<Mesh>> allMeshes;

			//---------- Setters ----------//
			void setMaterialOffset(int materialOffset) { _materialOffset = materialOffset; }

			//---------- Getters ----------//
			Material getMaterial() const { return _material; }
			int getMaterialOffset() const { return _materialOffset; }
			std::string getMeshName() const { return _meshName; }
			std::shared_ptr<Mesh> getMesh() const { return _mesh; }
			unsigned getMeshIndex() const { return _mesh->getIndex(); }

		private:
			std::string _meshName;
			std::shared_ptr<Mesh> _mesh;
			Material _material;
			int _materialOffset;
	};
}

#endif// ATTA_MODEL_H
