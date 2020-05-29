#include "scene.h"
#include "sphere.h"
#include <cstring>

Scene::Scene(CommandPool* commandPool, std::vector<Model>&& models, std::vector<Texture>&& textures, bool usedForRayTracing) :
	_models(std::move(models)),
	_textures(std::move(textures))
{
	// Concatenate all the models
	std::vector<Vertex> vertices;
	std::vector<uint32_t> indices;
	std::vector<Material> materials;
	std::vector<glm::vec4> procedurals;
	std::vector<std::pair<glm::vec3, glm::vec3>> aabbs;
	std::vector<glm::uvec2> offsets;

	for (const auto& model : _models)
	{
		// Remember the index, vertex offsets.
		const auto indexOffset = static_cast<uint32_t>(indices.size());
		const auto vertexOffset = static_cast<uint32_t>(vertices.size());
		const auto materialOffset = static_cast<uint32_t>(materials.size());

		offsets.emplace_back(indexOffset, vertexOffset);

		// Copy model data one after the other.
		vertices.insert(vertices.end(), model.vertices().begin(), model.vertices().end());
		indices.insert(indices.end(), model.indices().begin(), model.indices().end());
		materials.insert(materials.end(), model.materials().begin(), model.materials().end());

		// Adjust the material id.
		for (size_t i = vertexOffset; i != vertices.size(); ++i)
		{
			vertices[i].materialIndex += materialOffset;
		}

		// Add optional procedurals.
		const auto sphere = dynamic_cast<const Sphere*>(model.procedural());
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

	const auto flag = usedForRayTracing ? VK_BUFFER_USAGE_STORAGE_BUFFER_BIT : 0;

	createDeviceBuffer(commandPool, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | flag, vertices, &_vertexBuffer, &_vertexBufferMemory);
	createDeviceBuffer(commandPool, VK_BUFFER_USAGE_INDEX_BUFFER_BIT | flag, indices, &_indexBuffer, &_indexBufferMemory);
	createDeviceBuffer(commandPool, VK_BUFFER_USAGE_STORAGE_BUFFER_BIT, materials, &_materialBuffer, &_materialBufferMemory);
	createDeviceBuffer(commandPool, VK_BUFFER_USAGE_STORAGE_BUFFER_BIT, offsets, &_offsetBuffer, &_offsetBufferMemory);

	createDeviceBuffer(commandPool, VK_BUFFER_USAGE_STORAGE_BUFFER_BIT, aabbs, &_aabbBuffer, &_aabbBufferMemory);
	createDeviceBuffer(commandPool, VK_BUFFER_USAGE_STORAGE_BUFFER_BIT, procedurals, &_proceduralBuffer, &_proceduralBufferMemory);

	// Upload all textures
	_textureImages.reserve(_textures.size());
	_textureImageViewHandles.resize(_textures.size());
	_textureSamplerHandles.resize(_textures.size());
	
	for (size_t i = 0; i != _textures.size(); ++i)
	{
		_textureImages.emplace_back(new TextureImage(commandPool, _textures[i]));
		_textureImageViewHandles[i] = _textureImages[i]->imageView()->handle();
		_textureSamplerHandles[i] = _textureImages[i]->sampler()->handle();
	}
}

Scene::~Scene()
{
	_textureSamplerHandles.clear();
	_textureImageViewHandles.clear();
	_textureImages.clear();
	delete _proceduralBuffer;
	delete _proceduralBufferMemory; // release memory after bound buffer has been destroyed
	delete _aabbBuffer;
	delete _aabbBufferMemory; // release memory after bound buffer has been destroyed
	delete _offsetBuffer;
	delete _offsetBufferMemory; // release memory after bound buffer has been destroyed
	delete _materialBuffer;
	delete _materialBufferMemory; // release memory after bound buffer has been destroyed
	delete _indexBuffer;
	delete _indexBufferMemory; // release memory after bound buffer has been destroyed
	delete _vertexBuffer;
	delete _vertexBufferMemory; // release memory after bound buffer has been destroyed
}

template <class T>
void Scene::copyFromStagingBuffer(CommandPool* commandPool, Buffer* dstBuffer, const std::vector<T>& content)
{
	const auto& device = commandPool->device();
	const auto contentSize = sizeof(content[0]) * content.size();

	// Create a temporary host-visible staging buffer.
	Buffer* stagingBuffer = new Buffer(device, contentSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT);
	auto stagingBufferMemory = stagingBuffer->allocateMemory(VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

	// Copy the host data into the staging buffer.
	const auto data = stagingBufferMemory.mapMemory(0, contentSize);
	std::memcpy(data, content.data(), contentSize);
	stagingBufferMemory.unmapMemory();

	// Copy the staging buffer to the device buffer.
	dstBuffer->copyFrom(commandPool, stagingBuffer, contentSize);

	// Delete the buffer before the memory
	delete stagingBuffer;
}

template <class T>
void Scene::createDeviceBuffer(
	CommandPool* commandPool,
	const VkBufferUsageFlags usage, 
	const std::vector<T>& content,
	Buffer** buffer,
	DeviceMemory** memory)
{
	const auto& device = commandPool->device();
	const auto contentSize = sizeof(content[0]) * content.size();

	*buffer = new Buffer(device, contentSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | usage);
	*memory = new DeviceMemory((*buffer)->allocateMemory(VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT));

	copyFromStagingBuffer(commandPool, *buffer, content);
}
