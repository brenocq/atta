#ifndef MODEL_H
#define MODEL_H

#include <memory>
#include "../vulkan/device.h"
#include "../vulkan/deviceMemory.h"
#include "../vulkan/commandPool.h"
#include "../vulkan/tinyObjLoader.h"
#include "cornellBox.h"
#include "procedural.h"
#include "sphere.h"

class Model
{
public:
	Model() = default;
	Model(const Model&) = default;
	Model(Model&&) = default;
	~Model() = default;

	static Model loadModel(const std::string& filename);
	static Model createCornellBox(const float scale);
	static Model createBox(const glm::vec3& p0, const glm::vec3& p1, const Material& material);
	static Model createSphere(const glm::vec3& center, float radius, const Material& material, bool isProcedural);
	
	Model& operator = (const Model&) = delete;
	Model& operator = (Model&&) = delete;


	void setMaterial(const Material& material);
	void transform(const glm::mat4& transform);

	const std::vector<Vertex>& vertices() const { return _vertices; }
	const std::vector<uint32_t>& indices() const { return _indices; }
	const std::vector<Material>& materials() const { return _materials; }

	Procedural* procedural() const { return _procedural; }

	uint32_t numberOfVertices() const { return static_cast<uint32_t>(_vertices.size()); }
	uint32_t numberOfIndices() const { return static_cast<uint32_t>(_indices.size()); }
	uint32_t numberOfMaterials() const { return static_cast<uint32_t>(_materials.size()); }

private:

	Model(std::vector<Vertex>&& vertices, std::vector<uint32_t>&& indices, std::vector<Material>&& materials, Procedural* procedural);

	std::vector<Vertex> _vertices;
	std::vector<uint32_t> _indices;
	std::vector<Material> _materials;
	Procedural* _procedural;
};

#endif// MODEL_H
