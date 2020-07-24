//--------------------------------------------------
// Robot Simulator
// model.cpp
// Date: 08/07/2020
// By Breno Cunha Queiroz
//--------------------------------------------------
#include "model.h"
#include <chrono>
#include "sphere.h"

Model::Model(std::string fileName):
	_fileName(fileName), _procedural(nullptr)
{
	std::cout << std::endl << BOLDGREEN << "[Model]" << RESET << GREEN << " Loading model " << WHITE << fileName << RESET << std::endl;
	//loadTexture();
	loadModel();
}

Model::Model(std::vector<Vertex>&& vertices, std::vector<uint32_t>&& indices, std::vector<Material>&& materials, Procedural* procedural):
	_vertices(std::move(vertices)), 
	_indices(std::move(indices)),
	_materials(std::move(materials)),
	_procedural(procedural)
{
}

Model::~Model()
{
	if(_texture != nullptr)
	{
		delete _texture;
		_texture = nullptr;
	}

}

void Model::loadTexture(Device* device, CommandPool* commandPool)
{
	std::chrono::steady_clock::time_point begin;
	std::chrono::steady_clock::time_point end;
	std::string texturePath = "assets/models/"+_fileName+"/"+_fileName+".png";

	std::cout << GREEN << "\tLoading Texture... " << WHITE;

	begin = std::chrono::steady_clock::now();
	_texture = new Texture(device, commandPool, texturePath);
	end = std::chrono::steady_clock::now();


	std::cout << WHITE
		<< std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count()
		<< "ms" RESET << std::endl;
}

void Model::loadModel()
{
	std::chrono::steady_clock::time_point begin;
	std::chrono::steady_clock::time_point end;
	std::string modelPath = "assets/models/"+_fileName+"/"+_fileName+".obj";

	std::cout << GREEN << "\tLoading Vertices... " << WHITE;
	begin = std::chrono::steady_clock::now();

	tinyobj::ObjReader objReader;
	
	if (!objReader.ParseFromFile(modelPath))
	{
		std::cout << std::endl << BOLDRED << "[Model]" << RED << " Failed to load model" + modelPath + ": " + objReader.Error()<< RESET << std::endl;
		exit(1);
	}

	if (!objReader.Warning().empty())
	{
		std::cout << std::endl << BOLDYELLOW << "[Model]" << YELLOW << objReader.Warning() << RESET << std::endl << std::flush;
	}

	//---------- Materials ----------//
	for(const auto& material : objReader.GetMaterials())
	{
		//Material m = {};

		//m.diffuse = glm::vec4(material.diffuse[0], material.diffuse[1], material.diffuse[2], 1.0);
		//m.diffuseTextureId = -1;
		Material m = Material::diffuseLight(glm::vec3(material.diffuse[0], material.diffuse[1], material.diffuse[2]), -1);

		_materials.emplace_back(m);
	}

	if(_materials.empty())
	{
		Material m{};

		m.diffuse = glm::vec4(0.7f, 0.7f, 0.7f, 1.0);
		m.diffuseTextureId = -1;

		_materials.emplace_back(m);
	}

	//---------- Geometry ----------//
	const auto& objAttrib = objReader.GetAttrib();

	std::unordered_map<Vertex, uint32_t> uniqueVertices(objAttrib.vertices.size());
	size_t faceId = 0;

	for (const auto& shape : objReader.GetShapes())
	{
		const auto& mesh = shape.mesh;

		for (const auto& index : mesh.indices)
		{
			Vertex vertex = {};

			vertex.pos =
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
				uniqueVertices[vertex] = static_cast<uint32_t>(_vertices.size());
				_vertices.push_back(vertex);
			}

			_indices.push_back(uniqueVertices[vertex]);
		}
	}

	// If the model did not specify normals, then create smooth normals that conserve the same number of vertices.
	// Using flat normals would mean creating more vertices than we currently have, so for simplicity and better visuals we don't do it.
	// See https://stackoverflow.com/questions/12139840/obj-file-averaging-normals.
	if (objAttrib.normals.empty())
	{
		std::vector<glm::vec3> normals(_vertices.size());
		
		for (size_t i = 0; i < _indices.size(); i += 3)
		{
			const auto normal = glm::normalize(glm::cross(
				glm::vec3(_vertices[_indices[i + 1]].pos) - glm::vec3(_vertices[_indices[i]].pos),
				glm::vec3(_vertices[_indices[i + 2]].pos) - glm::vec3(_vertices[_indices[i]].pos)));

			_vertices[_indices[i + 0]].normal += normal;
			_vertices[_indices[i + 1]].normal += normal;
			_vertices[_indices[i + 2]].normal += normal;			
		}

		for(auto& vertex : _vertices)
		{
			vertex.normal = glm::normalize(vertex.normal);
		}
	}

	//tinyobj::attrib_t attrib;
    //std::vector<tinyobj::shape_t> shapes;
    //std::vector<tinyobj::material_t> materials;
    //std::string warn, err;

    //if(!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, modelPath.c_str()))
	//{
	//	std::cout << BOLDRED << "[Model]" << RESET << RED << warn + err << RESET << std::endl;
	//	exit(1);
    //}

	//std::unordered_map<Vertex, uint32_t> uniqueVertices{};
	//for(const auto& shape : shapes)
	//{
	//	for(const auto& index : shape.mesh.indices)
	//	{
	//		Vertex vertex{};

	//		vertex.pos = {
	//			attrib.vertices[3 * index.vertex_index + 0],
	//			attrib.vertices[3 * index.vertex_index + 1],
	//			attrib.vertices[3 * index.vertex_index + 2]
	//		};

	//		//vertex.texCoord = {
	//		//	attrib.texcoords[2 * index.texcoord_index + 0],
	//		//	attrib.texcoords[2 * index.texcoord_index + 1]
	//		//};
	//		vertex.texCoord = {
	//			attrib.texcoords[2 * index.texcoord_index + 0],
	//			1.0f - attrib.texcoords[2 * index.texcoord_index + 1]
	//		};

	//		vertex.color = {1.0f, 1.0f, 1.0f};

	//		_vertices.push_back(vertex);

	//		if (uniqueVertices.count(vertex) == 0) {
	//			uniqueVertices[vertex] = static_cast<uint32_t>(_vertices.size());
	//			_vertices.push_back(vertex);
	//		}
	//		_indices.push_back(uniqueVertices[vertex]);
	//	}
	//}

	//// Materials
	//for(const auto& material : materials)
	//{
	//	Material m{};
	//	m.diffuse = glm::vec4(material.diffuse[0], material.diffuse[1], material.diffuse[2], 1.0);
	//	m.diffuseTextureId = -1;

	//	_materials.emplace_back(m);
	//}

	//if(_materials.empty())
	//{
	//	Material m{};

	//	m.diffuse = glm::vec4(0.7f, 0.7f, 0.7f, 1.0);
	//	m.diffuseTextureId = -1;

	//	_materials.emplace_back(m);
	//}

	end = std::chrono::steady_clock::now();

	std::cout << WHITE
		<< std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count()
		<< "ms" 
		<< " (" << _vertices.size() << " vertices, " << _indices.size() << " indices)"	
		<< RESET << std::endl;
}


Model* Model::createSphere(const glm::vec3& center, float radius, const Material& material, const bool isProcedural)
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

			const glm::vec3 position(
				center.x + v * std::sin(i0),
				center.y + z,
				center.z + v * std::cos(i0));
			
			const glm::vec3 normal(
				n0 * std::sin(i0),
				n1,
				n0 * std::cos(i0));

			const glm::vec2 texCoord(
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

	return new Model(
		std::move(vertices),
		std::move(indices),
		std::vector<Material>{material},
		isProcedural ? new Sphere(center, radius) : nullptr);
}
