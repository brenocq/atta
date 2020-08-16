//--------------------------------------------------
// Robot Simulator
// scene.cpp
// Date: 16/07/2020
// By Breno Cunha Queiroz
//--------------------------------------------------
#include "scene.h"
#include <memory>
#include "vulkan/vertex.h"
#include "vulkan/material.h"
#include "vulkan/sphere.h"
#include "vulkan/buffer.h"
#include "vulkan/device.h"

Scene::Scene()
{
	//_physicsEngine = nullptr;
	_physicsEngine = new PhysicsEngine();
}

Scene::~Scene()
{
	if(_physicsEngine != nullptr)
	{
		delete _physicsEngine;
		_physicsEngine = nullptr;
	}

	if(_vertexBuffer != nullptr)
	{
		delete _vertexBuffer;
		_vertexBuffer = nullptr;
	}

	if(_indexBuffer != nullptr)
	{
		delete _indexBuffer;
		_indexBuffer = nullptr;
	}

	if(_materialBuffer != nullptr)
	{
		delete _materialBuffer;
		_materialBuffer = nullptr;
	}

	if(_offsetBuffer != nullptr)
	{
		delete _offsetBuffer;
		_offsetBuffer = nullptr;
	}

	if(_aabbBuffer != nullptr)
	{
		delete _aabbBuffer;
		_aabbBuffer = nullptr;
	}
	
	if(_proceduralBuffer != nullptr)
	{
		delete _proceduralBuffer;
		_proceduralBuffer = nullptr;
	}

	if(_lineVertexBuffer != nullptr)
	{
		delete _lineVertexBuffer;
		_lineVertexBuffer = nullptr;
	}

	if(_lineIndexBuffer != nullptr)
	{
		delete _lineIndexBuffer;
		_lineIndexBuffer = nullptr;
	}

	for(auto model : _models)
	{
		delete model;
		model = nullptr;
	}

	for(auto object : _objects)
	{
		delete object;
		object = nullptr;
	}

	// TODO delete when texture isnt deleted by the model
	for(auto texture : _textures)
	{
		delete texture;
		texture = nullptr;
	}
}

void Scene::loadObject(std::string fileName)
{
	// Load model to memory
	_models.push_back(new Model(fileName));
}

void Scene::addObject(Object* object)
{
	_objects.push_back(object);
	_physicsEngine->addRigidBody(_objects.back()->getObjectPhysics()->getRigidBody());
}

void Scene::createBuffers(CommandPool* commandPool)
{
	_commandPool = commandPool;
	Device* device = commandPool->getDevice();
	_proceduralBuffer = nullptr;

	_textures.push_back(new Texture(device, _commandPool, "assets/models/cube_multi/cube_multi.png"));

	// Concatenate all the models
	std::vector<Vertex> vertices;
	std::vector<uint32_t> indices;
	std::vector<Material> materials;
	std::vector<glm::uvec2> offsets;
	std::vector<std::pair<glm::vec3, glm::vec3>> aabbs;
	std::vector<glm::vec4> procedurals;

	for(const auto model : _models)
	{
		// Remember the index, vertex offsets.
		const uint32_t indexOffset = static_cast<uint32_t>(indices.size());
		const uint32_t vertexOffset = static_cast<uint32_t>(vertices.size());
		const auto materialOffset = static_cast<uint32_t>(materials.size());

		offsets.emplace_back(indexOffset, vertexOffset);

		// Copy model data one after the other.
		vertices.insert(vertices.end(), model->getVertices().begin(), model->getVertices().end());
		indices.insert(indices.end(), model->getIndices().begin(), model->getIndices().end());
		materials.insert(materials.end(), model->getMaterials().begin(), model->getMaterials().end());

		// Adjust the material id.
		for(size_t i = vertexOffset; i != vertices.size(); i++)
		{
			vertices[i].materialIndex += materialOffset;
		}

		// Add optional procedurals
		const auto sphere = dynamic_cast<const Sphere*>(model->getProcedural());
		if (sphere != nullptr)
		{
			aabbs.push_back(sphere->boundingBox());
			procedurals.emplace_back(sphere->center, sphere->radius);
		}
		else
		{
			aabbs.emplace_back();
			procedurals.emplace_back();
		}
	}

	// Ray tracing flasg
	const auto flag = VK_BUFFER_USAGE_STORAGE_BUFFER_BIT;
	std::pair<std::vector<Vertex>, std::vector<uint32_t>> grid = gridLines();
	_lineIndexCount = grid.second.size();

	createSceneBuffer(_vertexBuffer, 		VK_BUFFER_USAGE_VERTEX_BUFFER_BIT|flag, vertices);
	createSceneBuffer(_indexBuffer, 		VK_BUFFER_USAGE_INDEX_BUFFER_BIT|flag, 	indices);
	createSceneBuffer(_materialBuffer, 		VK_BUFFER_USAGE_STORAGE_BUFFER_BIT,		materials);
	createSceneBuffer(_offsetBuffer, 		VK_BUFFER_USAGE_STORAGE_BUFFER_BIT, 	offsets);
	createSceneBuffer(_aabbBuffer, 			VK_BUFFER_USAGE_STORAGE_BUFFER_BIT, 	aabbs);
	createSceneBuffer(_proceduralBuffer, 	VK_BUFFER_USAGE_STORAGE_BUFFER_BIT, 	procedurals);
	createSceneBuffer(_lineVertexBuffer, 	VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, 	grid.first);
	createSceneBuffer(_lineIndexBuffer, 	VK_BUFFER_USAGE_INDEX_BUFFER_BIT, 	grid.second);
}

void Scene::updatePhysics(float dt)
{
	_physicsEngine->stepSimulation(dt);
}


template <class T>
void Scene::createSceneBuffer(Buffer*& buffer,
		const VkBufferUsageFlags usage, 
		const std::vector<T>& content)
{
	const int size = sizeof(content[0]) * content.size();

	buffer = new Buffer(_commandPool->getDevice(), 
			size, 
			VK_BUFFER_USAGE_TRANSFER_DST_BIT | usage, 
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

	copyFromStagingBuffer(buffer, content);
}

template <class T>
void Scene::copyFromStagingBuffer(Buffer* dstBuffer, const std::vector<T>& content)
{
	Device* device = _commandPool->getDevice();
	const auto contentSize = sizeof(content[0]) * content.size();

	// Create a temporary host-visible staging buffer.
	Buffer* stagingBuffer = new Buffer(
			device, 
			contentSize, 
			VK_BUFFER_USAGE_TRANSFER_SRC_BIT, 
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);


	// Copy the host data into the staging buffer.
	void* data = stagingBuffer->mapMemory(0, contentSize);
	std::memcpy(data, content.data(), contentSize);
	stagingBuffer->unmapMemory();

	// Copy the staging buffer to the device buffer.
	dstBuffer->copyFrom(_commandPool, stagingBuffer->handle(), contentSize);

	// Delete the buffer before the memory
	delete stagingBuffer;
	stagingBuffer = nullptr;
}


std::pair<std::vector<Vertex>, std::vector<uint32_t>> Scene::gridLines()
{
	int gridSize = 100;
	const glm::vec3 gridColor = {0.35, 0.35, 0.35};
	const glm::vec2 texCoord(0,0);
	std::vector<Vertex> vertices;
	std::vector<uint32_t> indices;

	// X lines
	for(int i=-gridSize;i<gridSize;i++)
	{
		const glm::vec3 pos1(
			-gridSize,
			0,
			i);

		const glm::vec3 pos2(
			gridSize,
			0,
			i);
		
		vertices.push_back(Vertex{ pos1, gridColor, texCoord, 0 });
		vertices.push_back(Vertex{ pos2, gridColor, texCoord, 0 });
		indices.push_back(indices.size());
		indices.push_back(indices.size());
	}

	// Z lines
	for(int i=-gridSize;i<gridSize;i++)
	{
		const glm::vec3 pos1(
			i,
			0,
			-gridSize);

		const glm::vec3 pos2(
			i,
			0,
			gridSize);
		
		vertices.push_back(Vertex{ pos1, gridColor, texCoord, 0 });
		vertices.push_back(Vertex{ pos2, gridColor, texCoord, 0 });
		indices.push_back(indices.size());
		indices.push_back(indices.size());
	}

	return make_pair(vertices, indices);
}
