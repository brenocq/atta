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

	const std::vector<Vertex>& getVertices() const { return _vertices; };
	const std::vector<uint32_t>& getIndices() const { return _indices; };
	const std::vector<Material>& getMaterials() const { return _materials; };
	Procedural* getProcedural() const { return  _procedural; }

	static Model* createSphere(const glm::vec3& center, float radius, const Material& material, bool isProcedural);

	private:
	Model(std::vector<Vertex>&& vertices, std::vector<uint32_t>&& indices, std::vector<Material>&& materials, Procedural* procedural);
	void loadModel();

	VertexBuffer* _vertexBuffer;
	Procedural* _procedural;

	std::vector<Vertex> _vertices;
	std::vector<uint32_t> _indices;
	std::vector<Material> _materials;
	std::string _fileName;
};

#endif// MODEL_H
