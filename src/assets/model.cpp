#include "model.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtx/hash.hpp>
#include <chrono>
#include <experimental/filesystem>
#include <iostream>
#include <unordered_map>
#include <vector>

using namespace glm;

namespace std
{
	template<> struct hash<Vertex> final
	{
		size_t operator()(Vertex const& vertex) const noexcept
		{
			return
				combine(hash<vec3>()(vertex.position),
					combine(hash<vec3>()(vertex.normal),
						combine(hash<vec2>()(vertex.texCoord),
							hash<int>()(vertex.materialIndex))));
		}

	private:

		static size_t combine(size_t hash0, size_t hash1)
		{
			return hash0 ^ (hash1 + 0x9e3779b9 + (hash0 << 6) + (hash0 >> 2));
		}
	};
}

Model::Model(std::vector<Vertex>&& vertices, std::vector<uint32_t>&& indices, std::vector<Material>&& materials, Procedural* procedural) :
	_vertices(std::move(vertices)), 
	_indices(std::move(indices)),
	_materials(std::move(materials)),
	_procedural(procedural)
{
}

Model Model::loadModel(const std::string& filename)
{
	std::cout << BOLDWHITE << "[Model] " << RESET << "Loading [" << filename << "]... ";

	const auto timer = std::chrono::high_resolution_clock::now();
	//const std::string materialPath = std::filesystem::path(filename).parent_path().string();
	
	tinyobj::ObjReader objReader;
	
	if (!objReader.ParseFromFile(filename))
	{
		std::cout << BOLDRED << "ERROR" << RESET << RED << "\n\tInfo: " << objReader.Error() << RESET << std::endl;
		exit(1);
	}

	if (!objReader.Warning().empty())
	{
		std::cout << YELLOW << "[Model] Warning: " << objReader.Warning() << RESET << std::endl;
	}

	// Materials
	std::vector<Material> materials;

	for (const auto& material : objReader.GetMaterials())
	{
		Material m{};

		m.diffuse = vec4(material.diffuse[0], material.diffuse[1], material.diffuse[2], 1.0);
		m.diffuseTextureId = -1;

		materials.emplace_back(m);
	}

	if (materials.empty())
	{
		Material m{};

		m.diffuse = vec4(0.7f, 0.7f, 0.7f, 1.0);
		m.diffuseTextureId = -1;

		materials.emplace_back(m);
	}

	// Geometry
	const auto& objAttrib = objReader.GetAttrib();

	std::vector<Vertex> vertices;
	std::vector<uint32_t> indices;
	std::unordered_map<Vertex, uint32_t> uniqueVertices(objAttrib.vertices.size());
	size_t faceId = 0;

	for (const auto& shape : objReader.GetShapes())
	{
		const auto& mesh = shape.mesh;

		for (const auto& index : mesh.indices)
		{
			Vertex vertex = {};

			vertex.position =
			{
				objAttrib.vertices[3 * index.vertex_index + 0],
				objAttrib.vertices[3 * index.vertex_index + 1],
				objAttrib.vertices[3 * index.vertex_index + 2],
			};

			if (!objAttrib.normals.empty())
			{
				vertex.normal =
				{
					objAttrib.normals[3 * index.normal_index + 0],
					objAttrib.normals[3 * index.normal_index + 1],
					objAttrib.normals[3 * index.normal_index + 2]
				};
			}

			if (!objAttrib.texcoords.empty())
			{
				vertex.texCoord =
				{
					objAttrib.texcoords[2 * index.texcoord_index + 0],
					1 - objAttrib.texcoords[2 * index.texcoord_index + 1]
				};
			}

			vertex.materialIndex = std::max(0, mesh.material_ids[faceId++ / 3]);

			if (uniqueVertices.count(vertex) == 0)
			{
				uniqueVertices[vertex] = static_cast<uint32_t>(vertices.size());
				vertices.push_back(vertex);
			}

			indices.push_back(uniqueVertices[vertex]);
		}
	}

	// If the model did not specify normals, then create smooth normals that conserve the same number of vertices.
	// Using flat normals would mean creating more vertices than we currently have, so for simplicity and better visuals we don't do it.
	// See https://stackoverflow.com/questions/12139840/obj-file-averaging-normals.
	if (objAttrib.normals.empty())
	{
		std::vector<vec3> normals(vertices.size());
		
		for (size_t i = 0; i < indices.size(); i += 3)
		{
			const auto normal = normalize(cross(
				vec3(vertices[indices[i + 1]].position) - vec3(vertices[indices[i]].position),
				vec3(vertices[indices[i + 2]].position) - vec3(vertices[indices[i]].position)));

			vertices[indices[i + 0]].normal += normal;
			vertices[indices[i + 1]].normal += normal;
			vertices[indices[i + 2]].normal += normal;			
		}

		for (auto& vertex : vertices)
		{
			vertex.normal = normalize(vertex.normal);
		}
	}

	const auto elapsed = std::chrono::duration<float, std::chrono::seconds::period>(std::chrono::high_resolution_clock::now() - timer).count();

	std::cout << BOLDGREEN << "OK" << RESET << std::endl;
	std::cout << "\tInfo: " << objAttrib.vertices.size() << " vertices, " << uniqueVertices.size() << " unique vertices, " << materials.size() << " materials" << std::endl;
	std::cout << "\tTime: " << elapsed << "s" << std::endl;

	return Model(std::move(vertices), std::move(indices), std::move(materials), nullptr);
}

Model Model::createCornellBox(const float scale)
{
	std::vector<Vertex> vertices;
	std::vector<uint32_t> indices;
	std::vector<Material> materials;

	CornellBox::create(scale, vertices, indices, materials);

	return Model(
		std::move(vertices),
		std::move(indices),
		std::move(materials),
		nullptr
	);
}

Model Model::createBox(const vec3& p0, const vec3& p1, const Material& material)
{
	std::vector<Vertex> vertices = 
	{
		Vertex{vec3(p0.x, p0.y, p0.z), vec3(-1, 0, 0), vec2(0), 0},
		Vertex{vec3(p0.x, p0.y, p1.z), vec3(-1, 0, 0), vec2(0), 0},
		Vertex{vec3(p0.x, p1.y, p1.z), vec3(-1, 0, 0), vec2(0), 0},
		Vertex{vec3(p0.x, p1.y, p0.z), vec3(-1, 0, 0), vec2(0), 0},

		Vertex{vec3(p1.x, p0.y, p1.z), vec3(1, 0, 0), vec2(0), 0},
		Vertex{vec3(p1.x, p0.y, p0.z), vec3(1, 0, 0), vec2(0), 0},
		Vertex{vec3(p1.x, p1.y, p0.z), vec3(1, 0, 0), vec2(0), 0},
		Vertex{vec3(p1.x, p1.y, p1.z), vec3(1, 0, 0), vec2(0), 0},

		Vertex{vec3(p1.x, p0.y, p0.z), vec3(0, 0, -1), vec2(0), 0},
		Vertex{vec3(p0.x, p0.y, p0.z), vec3(0, 0, -1), vec2(0), 0},
		Vertex{vec3(p0.x, p1.y, p0.z), vec3(0, 0, -1), vec2(0), 0},
		Vertex{vec3(p1.x, p1.y, p0.z), vec3(0, 0, -1), vec2(0), 0},

		Vertex{vec3(p0.x, p0.y, p1.z), vec3(0, 0, 1), vec2(0), 0},
		Vertex{vec3(p1.x, p0.y, p1.z), vec3(0, 0, 1), vec2(0), 0},
		Vertex{vec3(p1.x, p1.y, p1.z), vec3(0, 0, 1), vec2(0), 0},
		Vertex{vec3(p0.x, p1.y, p1.z), vec3(0, 0, 1), vec2(0), 0},

		Vertex{vec3(p0.x, p0.y, p0.z), vec3(0, -1, 0), vec2(0), 0},
		Vertex{vec3(p1.x, p0.y, p0.z), vec3(0, -1, 0), vec2(0), 0},
		Vertex{vec3(p1.x, p0.y, p1.z), vec3(0, -1, 0), vec2(0), 0},
		Vertex{vec3(p0.x, p0.y, p1.z), vec3(0, -1, 0), vec2(0), 0},

		Vertex{vec3(p1.x, p1.y, p0.z), vec3(0, 1, 0), vec2(0), 0},
		Vertex{vec3(p0.x, p1.y, p0.z), vec3(0, 1, 0), vec2(0), 0},
		Vertex{vec3(p0.x, p1.y, p1.z), vec3(0, 1, 0), vec2(0), 0},
		Vertex{vec3(p1.x, p1.y, p1.z), vec3(0, 1, 0), vec2(0), 0},
	};

	std::vector<uint32_t> indices =
	{
		0, 1, 2, 0, 2, 3,
		4, 5, 6, 4, 6, 7,
		8, 9, 10, 8, 10, 11,
		12, 13, 14, 12, 14, 15,
		16, 17, 18, 16, 18, 19,
		20, 21, 22, 20, 22, 23
	};

	return Model(
		std::move(vertices),
		std::move(indices),
		std::vector<Material>{material},
		nullptr);
}

Model Model::createSphere(const vec3& center, float radius, const Material& material, const bool isProcedural)
{
	const int slices = 32;
	const int stacks = 16;
	
	std::vector<Vertex> vertices;
	std::vector<uint32_t> indices;

	const float pi = 3.14159265358979f;
	
	for (int j = 0; j <= stacks; ++j) 
	{
		const float j0 = pi * j / stacks;

		// Vertex
		const float v = radius * -std::sin(j0);
		const float z = radius * std::cos(j0);
		
		// Normals		
		const float n0 = -std::sin(j0);
		const float n1 = std::cos(j0);

		for (int i = 0; i <= slices; ++i) 
		{
			const float i0 = 2 * pi * i / slices;

			const vec3 position(
				center.x + v * std::sin(i0),
				center.y + z,
				center.z + v * std::cos(i0));
			
			const vec3 normal(
				n0 * std::sin(i0),
				n1,
				n0 * std::cos(i0));

			const vec2 texCoord(
				static_cast<float>(i) / slices,
				static_cast<float>(j) / stacks);

			vertices.push_back(Vertex{ position, normal, texCoord, 0 });
		}
	}

	for (int j = 0; j < stacks; ++j)
	{
		for (int i = 0; i < slices; ++i)
		{
			const auto j0 = (j + 0) * (slices + 1);
			const auto j1 = (j + 1) * (slices + 1);
			const auto i0 = i + 0;
			const auto i1 = i + 1;
			
			indices.push_back(j0 + i0);
			indices.push_back(j1 + i0);
			indices.push_back(j1 + i1);
			
			indices.push_back(j0 + i0);
			indices.push_back(j1 + i1);
			indices.push_back(j0 + i1);
		}
	}

	return Model(
		std::move(vertices),
		std::move(indices),
		std::vector<Material>{material},
		isProcedural ? new Sphere(center, radius) : nullptr);
}

void Model::setMaterial(const Material& material)
{
	if (_materials.size() != 1)
	{
		std::cout << BOLDRED << "[Model] Cannot change material on a multi-material model " << RESET << std::endl;
		exit(1);
	}

	_materials[0] = material;
}

void Model::transform(const mat4& transform)
{
	const auto transformIT = inverseTranspose(transform);

	for (auto& vertex : _vertices)
	{
		vertex.position = transform * vec4(vertex.position, 1);
		vertex.normal = transformIT * vec4(vertex.normal, 0);
	}
}
