//--------------------------------------------------
// Atta Graphics
// vulkanCore.cpp
// Date: 2021-01-11
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/graphics/vulkan/vulkanCore.h>
#include <atta/helpers/log.h>
#include <atta/graphics/core/model.h>
#include <atta/graphics/core/material.h>
#include <atta/graphics/core/objectInfo.h>
#include <atta/graphics/vulkan/stagingBuffer.h>

namespace atta::vk
{
	VulkanCore::VulkanCore()
	{
		_instance = std::make_shared<Instance>();
		_debugMessenger = std::make_unique<DebugMessenger>(_instance);
		_physicalDevice = std::make_shared<PhysicalDevice>(_instance);
		_device = std::make_shared<Device>(_physicalDevice);
		//_commandPool = std::make_shared<CommandPool>(_device, CommandPool::DEVICE_QUEUE_FAMILY_GRAPHICS, CommandPool::QUEUE_THREAD_MANAGER, VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT);
	}

	VulkanCore::~VulkanCore()
	{

	}

	void VulkanCore::createBuffers(std::shared_ptr<Scene> scene, std::shared_ptr<CommandPool> commandPool)
	{
		// Concatenate objects to populate host memory buffers 
		std::vector<Vertex> vertices;
		std::vector<uint32_t> indices;
		std::vector<Material> materials;
		std::vector<ObjectInfo> objectInfos;
		std::vector<Light> lights;

		// Populate vertices/indices
		for(auto m : Model::allMeshes)
		{
			std::shared_ptr<Mesh> mesh = m.second.lock();

			// Remember the vertex and index offsets
			const uint32_t vertexOffset = static_cast<uint32_t>(vertices.size());
			const uint32_t indexOffset = static_cast<uint32_t>(indices.size());
			mesh->setVerticesOffset(vertexOffset);
			mesh->setIndicesOffset(indexOffset);

			// TODO vertex.materialIndex not supported yet
			std::vector<Vertex> meshVertices = mesh->getVertices();
			std::vector<uint32_t> meshIndices = mesh->getIndices();
			//for(auto vertex : meshVertices)
			//	vertex.materialIndex+=meshMaterial;
			//meshMaterial++;

			vertices.insert(vertices.end(), meshVertices.begin(), meshVertices.end());
			indices.insert(indices.end(), meshIndices.begin(), meshIndices.end());
		}

		// Populate materials/objectInfos
		for(auto object : scene->getObjects())
		{
			if(object->isLight()) continue;

			static int materialOffset = 0;
			std::shared_ptr<Model> model = object->getModel();
			model->setMaterialOffset(materialOffset);
			materials.push_back(model->getMaterial());
			Log::debug("VulkanCode", "Added $0", model->getMaterial().toString());

			ObjectInfo obji;
			obji.indexOffset = model->getMesh()->getIndicesOffset();
			obji.vertexOffset = model->getMesh()->getVerticesOffset();
			obji.materialOffset = materialOffset;
			obji.transform = transpose(object->getModelMat());
			objectInfos.push_back(obji);

			materialOffset++;
		}

		// Populate lights
		lights = scene->getLights();

		//---------- Create device buffers ----------//
		_vertexBuffer = createBufferMemory(commandPool,
				VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT, 
				vertices);
		_indexBuffer = createBufferMemory(commandPool,
				VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT, 
				indices);
		_materialBuffer = createBufferMemory(commandPool,VK_BUFFER_USAGE_STORAGE_BUFFER_BIT, materials);
		_objectInfoBuffer = createBufferMemory(commandPool,VK_BUFFER_USAGE_STORAGE_BUFFER_BIT, objectInfos);
		_lightBuffer = createBufferMemory(commandPool,VK_BUFFER_USAGE_STORAGE_BUFFER_BIT, lights);

		//---------- Create textures ----------//
		for(auto& textureInfo : atta::Texture::textureInfos())
		{
			switch(textureInfo.type)
			{
				case atta::Texture::TYPE_FILE:
					{
						_textures.push_back(std::make_shared<vk::Texture>(_device, commandPool, textureInfo.fileName));
						textureInfo.vkTexture = _textures.back();
					}
					break;
				case atta::Texture::TYPE_BUFFER:
					{
						_textures.push_back(std::make_shared<vk::Texture>(
									_device, 
									commandPool, 
									(void*)textureInfo.data, 
									(VkExtent2D){textureInfo.width, textureInfo.height}, 
									textureInfo.format));
						textureInfo.vkTexture = _textures.back();
					}
					break;
				default:
					{
						Log::warning("VulkanCore", "Textures should not have NONE type.");
					}
			}
		}
		if(_textures.size()==0)
		{
			// Texture buffer can't be empty, push one texture
			_textures.push_back(std::make_shared<vk::Texture>(_device, commandPool, "/usr/include/atta/assets/textures/icon.png"));
		}
	}

	void VulkanCore::updateBuffers(std::shared_ptr<Scene> scene, std::shared_ptr<CommandPool> commandPool)
	{

	}

	template <class T>
	std::shared_ptr<Buffer> VulkanCore::createBufferMemory(std::shared_ptr<CommandPool> commandPool,
			const VkBufferUsageFlags usage, 
			std::vector<T>& content)
	{
		int size = sizeof(T) * content.size();

		VkMemoryAllocateFlags allocateFlags = 0;
		if(usage & VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT)
			allocateFlags = VK_MEMORY_ALLOCATE_DEVICE_ADDRESS_BIT_KHR;
	
		// Create buffer (local memory)
		std::shared_ptr<Buffer> buffer = std::make_shared<Buffer>(
				_device, 
				size!=0?size:sizeof(T), 
				VK_BUFFER_USAGE_TRANSFER_DST_BIT | usage, 
				VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
				allocateFlags);

		if(size > 0)
		{
			// Create staging buffer (host memory)
			std::shared_ptr<StagingBuffer> stagingBuffer = std::make_shared<StagingBuffer>(_device, content.data(), size);

			// Copy from host memory to device memory
			buffer->copyFrom(commandPool, stagingBuffer->handle(), size);
		}

		return buffer;
	}
}
