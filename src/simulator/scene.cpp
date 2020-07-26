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

Scene::Scene(CommandPool* commandPool, std::vector<Model*> models, std::vector<Texture*> textures, bool enableRayTracing):
	_models(std::move(models)), _textures(std::move(textures))
{
	_commandPool = commandPool;
	_proceduralBuffer = nullptr;

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

	const auto flag = enableRayTracing ? VK_BUFFER_USAGE_STORAGE_BUFFER_BIT : 0;

	createSceneBuffer(_vertexBuffer, 		VK_BUFFER_USAGE_VERTEX_BUFFER_BIT|flag, vertices);
	createSceneBuffer(_indexBuffer, 		VK_BUFFER_USAGE_INDEX_BUFFER_BIT|flag, 	indices);
	createSceneBuffer(_materialBuffer, 		VK_BUFFER_USAGE_STORAGE_BUFFER_BIT,		materials);
	createSceneBuffer(_offsetBuffer, 		VK_BUFFER_USAGE_STORAGE_BUFFER_BIT, 	offsets);
	createSceneBuffer(_aabbBuffer, 			VK_BUFFER_USAGE_STORAGE_BUFFER_BIT, 	aabbs);
	createSceneBuffer(_proceduralBuffer, 	VK_BUFFER_USAGE_STORAGE_BUFFER_BIT, 	procedurals);
}

Scene::~Scene()
{
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

	for(auto model : _models)
	{
		delete model;
		model = nullptr;
	}

	// TODO delete when texture isnt deleted by the model
	//for(auto texture : _textures)
	//{
	//	delete texture;
	//	texture = nullptr;
	//}
}

void Scene::addModel(Model* model)
{
	_models.push_back(model);
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

	//// Copy the staging buffer to the device buffer.
	dstBuffer->copyFrom(_commandPool, stagingBuffer->handle(), contentSize);

	//// Delete the buffer before the memory
	delete stagingBuffer;
	stagingBuffer = nullptr;
}
