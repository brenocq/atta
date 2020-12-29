//--------------------------------------------------
// Robot Simulator
// model.h
// Date: 2020-07-08
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef MODEL_H
#define MODEL_H

#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include "defines.h"
#include "device.h"
#include "glm.h"
#include "commandPool.h"
#include "vertexBuffer.h"
#include "texture.h"
#include "tinyObjLoader.h"
#include "vertex.h"
#include "material.h"
#include "procedural.h"

class Model
{
	public:
		Model(std::string fileName);
		~Model();

		void transform(const glm::mat4& transform);

		//------------- Getters -------------//
		const std::vector<Vertex>& getVertices() const { return _vertices; };
		const std::vector<uint32_t>& getIndices() const { return _indices; };
		const std::vector<Material>& getMaterials() const { return _materials; };
		const std::vector<Texture*> getTextures(Device* device, CommandPool* commandPool);
		Procedural* getProcedural() const { return  _procedural; }
		int getModelIndex() const { return _modelIndex; }
		uint32_t getVertexOffset() const { return Model::vertexOffsets[_modelIndex]; }
		uint32_t getIndexOffset() const { return Model::indexOffsets[_modelIndex]; }
		uint32_t getVerticesSize() const { return Model::verticesSize[_modelIndex]; }
		uint32_t getIndicesSize() const { return Model::indicesSize[_modelIndex]; }
		std::string getFileName() const { return _fileName; }

	private:
		Model(std::vector<Vertex>&& vertices, std::vector<uint32_t>&& indices, std::vector<Material>&& materials, Procedural* procedural);
		void loadModel();
		void createSphereModel();

		// Model properties
		std::string _fileName;
		int _modelIndex;
		Procedural* _procedural;

		// Info about models already added
		static int textureId;
		static int qtyModels;
		static std::vector<std::string> currentModels;
		static std::vector<uint32_t> vertexOffsets;
		static std::vector<uint32_t> indexOffsets;
		static std::vector<uint32_t> verticesSize;
		static std::vector<uint32_t> indicesSize;

		// Helpers (Only used first time the object is added)
		std::vector<Vertex> _vertices;
		std::vector<uint32_t> _indices;
		std::vector<Material> _materials;
		std::vector<std::string> _textureNames;
};

#endif// MODEL_H
