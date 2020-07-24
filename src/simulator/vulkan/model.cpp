//--------------------------------------------------
// Robot Simulator
// model.cpp
// Date: 08/07/2020
// By Breno Cunha Queiroz
//--------------------------------------------------
#include "model.h"
#include <chrono>
#include "glm.h"

Model::Model(std::string fileName):
	_fileName(fileName)
{
	std::cout << std::endl << BOLDGREEN << "[Model]" << RESET << GREEN << " Loading model " << WHITE << fileName << RESET << std::endl;
	//loadTexture();
	loadModel();
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

	tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string warn, err;

	std::cout << GREEN << "\tLoading Vertices... " << WHITE;
	begin = std::chrono::steady_clock::now();

    if(!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, modelPath.c_str()))
	{
		std::cout << BOLDRED << "[Model]" << RESET << RED << warn + err << RESET << std::endl;
		exit(1);
    }

	std::unordered_map<Vertex, uint32_t> uniqueVertices{};
	for(const auto& shape : shapes)
	{
		for(const auto& index : shape.mesh.indices)
		{
			Vertex vertex{};

			vertex.pos = {
				attrib.vertices[3 * index.vertex_index + 0],
				attrib.vertices[3 * index.vertex_index + 1],
				attrib.vertices[3 * index.vertex_index + 2]
			};

			//vertex.texCoord = {
			//	attrib.texcoords[2 * index.texcoord_index + 0],
			//	attrib.texcoords[2 * index.texcoord_index + 1]
			//};
			vertex.texCoord = {
				attrib.texcoords[2 * index.texcoord_index + 0],
				1.0f - attrib.texcoords[2 * index.texcoord_index + 1]
			};

			vertex.color = {1.0f, 1.0f, 1.0f};

			_vertices.push_back(vertex);

			if (uniqueVertices.count(vertex) == 0) {
				uniqueVertices[vertex] = static_cast<uint32_t>(_vertices.size());
				_vertices.push_back(vertex);
			}
			_indices.push_back(uniqueVertices[vertex]);
		}
	}

	// Materials
	for(const auto& material : materials)
	{
		Material m{};
		m.diffuse = glm::vec4(material.diffuse[0], material.diffuse[1], material.diffuse[2], 1.0);
		m.diffuseTextureId = -1;

		_materials.emplace_back(m);
	}

	if(_materials.empty())
	{
		Material m{};

		m.diffuse = glm::vec4(0.7f, 0.7f, 0.7f, 1.0);
		m.diffuseTextureId = -1;

		_materials.emplace_back(m);
	}

	end = std::chrono::steady_clock::now();

	std::cout << WHITE
		<< std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count()
		<< "ms" 
		<< " (" << _vertices.size() << " vertices, " << _indices.size() << " indices)"	
		<< RESET << std::endl;
}
