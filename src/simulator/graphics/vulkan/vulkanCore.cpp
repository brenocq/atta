//--------------------------------------------------
// Atta Graphics
// vulkanCore.cpp
// Date: 2021-01-11
// By Breno Cunha Queiroz
//--------------------------------------------------
#include "vulkanCore.h"
#include "simulator/helpers/log.h"
#include "simulator/graphics/core/model.h"
#include "simulator/graphics/vulkan/stagingBuffer.h"

namespace atta::vk
{
	VulkanCore::VulkanCore()
	{
		_instance = std::make_shared<Instance>();
		_debugMessenger = std::make_unique<DebugMessenger>(_instance);
		_physicalDevice = std::make_shared<PhysicalDevice>(_instance);
		_device = std::make_shared<Device>(_physicalDevice);
		_commandPool = std::make_shared<CommandPool>(_device, VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT);
	}

	VulkanCore::~VulkanCore()
	{

	}

	void VulkanCore::createBuffers(std::shared_ptr<Scene> scene)
	{
		// Concatenate objects to populate host memory buffers 
		std::vector<Vertex> vertices;
		std::vector<uint32_t> indices;
		std::vector<Material> materials;
		std::vector<MeshOffset> offsets;

		int meshMaterial = 0;
		for(auto m : Model::allMeshes)
		{
			std::shared_ptr<Mesh> mesh = m.second.lock();

			// Remember the vertex and index offsets
			const uint32_t vertexOffset = static_cast<uint32_t>(vertices.size());
			const uint32_t indexOffset = static_cast<uint32_t>(indices.size());

			offsets.push_back({vertexOffset, indexOffset});
			mesh->setVerticesOffset(vertexOffset);
			mesh->setIndicesOffset(indexOffset);

			std::vector<Vertex> meshVertices = mesh->getVertices();
			for(auto vertex : meshVertices)
				vertex.materialIndex+=meshMaterial;
			meshMaterial++;

			vertices.insert(vertices.end(), meshVertices.begin(), meshVertices.end());
			indices.insert(indices.end(), mesh->getIndices().begin(), mesh->getIndices().end());
		}

		for(auto object : scene->getObjects())
		{
			std::shared_ptr<Model> model = object->getModel();
			materials.push_back(model->getMaterial());
		}

		//---------- Create device buffers ----------//
		createBufferMemory(_vertexBuffer, 	VK_BUFFER_USAGE_VERTEX_BUFFER_BIT|VK_BUFFER_USAGE_STORAGE_BUFFER_BIT, vertices);
		createBufferMemory(_indexBuffer, 	VK_BUFFER_USAGE_INDEX_BUFFER_BIT|VK_BUFFER_USAGE_STORAGE_BUFFER_BIT, indices);
		createBufferMemory(_materialBuffer, VK_BUFFER_USAGE_STORAGE_BUFFER_BIT,	materials);
	}

	void VulkanCore::updateBuffers(std::shared_ptr<Scene> scene)
	{

	}

	template <class T>
	void VulkanCore::createBufferMemory(
			std::shared_ptr<Buffer>& buffer,
			const VkBufferUsageFlags usage, 
			std::vector<T>& content)
	{
		int size = sizeof(content[0]) * content.size();

		// Create buffer (local memory)
		buffer = std::make_shared<Buffer>(
				_device, 
				size, 
				VK_BUFFER_USAGE_TRANSFER_DST_BIT | usage, 
				VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

		// Create staging buffer (host memory)
		std::shared_ptr<StagingBuffer> stagingBuffer = std::make_shared<StagingBuffer>(_device, content.data(), size);

		// Copy from host memory to device memory
		buffer->copyFrom(_commandPool, stagingBuffer->handle(), size);
	}
}
