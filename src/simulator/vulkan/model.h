//--------------------------------------------------
// Robot Simulator
// model.h
// Date: 08/07/2020
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
#include "commandPool.h"
#include "vertexBuffer.h"
#include "texture.h"
#include "tinyObjLoader.h"
#include "vertex.h"
#include "material.h"

class Model
{
	public:
	Model(std::string fileName);
	~Model();

	Texture* getTexture() const { return _texture; };
	const std::vector<Vertex>& getVertices() const { return _vertices; };
	const std::vector<uint32_t>& getIndices() const { return _indices; };
	const std::vector<Material>& getMaterials() const { return _materials; };

	void loadTexture(Device* device, CommandPool* commandPool);
	private:
	void loadModel();

	VertexBuffer* _vertexBuffer;
	Texture* _texture;

	std::vector<Vertex> _vertices;
	std::vector<uint32_t> _indices;
	std::vector<Material> _materials;
	std::string _fileName;
};

#endif// MODEL_H
