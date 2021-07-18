//--------------------------------------------------
// Atta Graphics
// vulkanCore.cpp
// Date: 2021-01-11
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/graphics/vulkan/vulkanCore.h>
#include <atta/helpers/log.h>
#include <atta/helpers/drawer.h>
#include <atta/graphics/core/model.h>
#include <atta/graphics/core/material.h>
#include <atta/graphics/core/objectInfo.h>
#include <atta/graphics/vulkan/stagingBuffer.h>
#include <atta/graphics/vulkan/compute/envIrradiance.h>
#include <atta/objects/lights/lights.h>
#include <queue>

namespace atta::vk
{
	VulkanCore::VulkanCore()
	{
		_instance = std::make_shared<Instance>();
		_debugMessenger = std::make_unique<DebugMessenger>(_instance);
		_physicalDevice = std::make_shared<PhysicalDevice>(_instance);
		_device = std::make_shared<Device>(_physicalDevice);
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

			vertices.insert(vertices.end(), meshVertices.begin(), meshVertices.end());
			indices.insert(indices.end(), meshIndices.begin(), meshIndices.end());
		}

		// Populate materials/objectInfos
		for(auto parentObject : scene->getObjects())
		{
			// Queue with object and its children
			std::queue<std::shared_ptr<Object>> objects;
			objects.push(parentObject);
			while(!objects.empty())
			{
				// Get next object
				std::shared_ptr<Object> object = objects.front();
				objects.pop();

				// Populate queue with its children
				std::vector<std::shared_ptr<Object>> children = object->getChildren();
				for(auto& child : children)
					objects.push(child);

				// Get object materials and set materialOffset
				static int materialOffset = 0;
				std::shared_ptr<Model> model = object->getModel();
				if(model==nullptr)
					continue;
				model->setMaterialOffset(materialOffset);

				int qtyMaterials;
				auto modelMaterialMap = model->getMaterials();
				// Only one material
				if(modelMaterialMap.count("atta::material"))
				{
					qtyMaterials = 1;
					materials.push_back(modelMaterialMap["atta::material"]);
				}
				// Multiple materials
				else
				{
					qtyMaterials = model->getMesh()->getMaterialNames().size();
					// For each material name in the .mtl file (loaded by mesh), 
					// try to push one material (if material with name not defined, load default material)
					for(const auto& materialName : model->getMesh()->getMaterialNames())
					{
						Log::debug("VulkanCore", "Material($0): $1", materialName, modelMaterialMap[materialName].toString());
						if(modelMaterialMap.count(materialName))
							materials.push_back(modelMaterialMap[materialName]);
						else
							materials.push_back(Material::diffuse({}));
					}
				}

				ObjectInfo obji;
				obji.indexOffset = model->getMesh()->getIndicesOffset();
				obji.vertexOffset = model->getMesh()->getVerticesOffset();
				obji.materialOffset = materialOffset;
				obji.transform = transpose(object->getModelMat());
				objectInfos.push_back(obji);

				materialOffset+=qtyMaterials;
			}
		}

		// Populate lights
		for(auto object : scene->getObjectsFlat())
		{
			//---------- Create lights from light objects ----------//
			// Light structs to send to the GPU memory
			if(object->isLight())
			{
				std::string type = object->getType();
				if(type == "PointLight")
				{
					std::shared_ptr<PointLight> l = std::static_pointer_cast<PointLight>(object);
					lights.push_back(Light::point(l->getPosition(), l->getIntensity()));
				}
				else if(type == "SpotLight")
				{
					std::shared_ptr<SpotLight> l = std::static_pointer_cast<SpotLight>(object);
					lights.push_back(Light::spot(l->getPosition(), l->getDirection(), l->getIntensity(), l->getFalloffStart(), l->getTotalWidth()));
				}
				if(type == "DistantLight")
				{
					std::shared_ptr<DistantLight> l = std::static_pointer_cast<DistantLight>(object);
					lights.push_back(Light::distant(l->getRadiance(), l->getDirection()));
				}
				if(type == "InfiniteLight")
				{
					std::shared_ptr<InfiniteLight> l = std::static_pointer_cast<InfiniteLight>(object);
					lights.push_back(Light::infinite(l->getRadiance(), l->getPosition(), l->getOrientation(), l->getPrecomputedPower(), l->getWorldRadius(), l->getTextureIndex(), l->getPdfTextureIndex(), l->getIrradianceTextureIndex()));
				}
				if(type == "TriangleMeshLight")
				{
					std::shared_ptr<TriangleMeshLight> l = std::static_pointer_cast<TriangleMeshLight>(object);
					std::vector<vec3> vertices = l->getVertices();
					for(int i=0;i<vertices.size()/3;i++)
					{
						lights.push_back(Light::areaTriangle(l->getRadiance(), vertices[i*3], vertices[i*3+1], vertices[i*3+2]));
					}
				}
			}
		}

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

		// Aux/Debug buffers
		_lineBuffer = createBufferMemory(commandPool,VK_BUFFER_USAGE_STORAGE_BUFFER_BIT, Drawer::getLines());
		_pointBuffer = createBufferMemory(commandPool,VK_BUFFER_USAGE_STORAGE_BUFFER_BIT, Drawer::getPoints());

		//---------- Create textures ----------//
		// Create main texture
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
				case atta::Texture::TYPE_PROCESSED:
					{
						switch(textureInfo.process.first) {
							case atta::Texture::PROCESS_CUBEMAP:
									// TODO
								break;
							case atta::Texture::PROCESS_ENV_IRRADIANCE:
								{
									// Compute env irradiance from another texture
									compute::EnvIrradiance::CreateInfo envIrrInfo {
										.device = _device,
										.commandPool = commandPool,
										.input = atta::Texture::textureInfos()[textureInfo.process.second].vkTexture.lock()
									};
									compute::EnvIrradiance envIrr(envIrrInfo);
									//envIrr.compute();

									// Add to textures buffer
									_textures.push_back(envIrr.getOutput());
									textureInfo.vkTexture = _textures.back();
								}
								break;
							default:
								Log::warning("VulkanCore", "Textures process should not have NONE type");
						}
					}
					break;
				default:
					{
						Log::warning("VulkanCore", "Textures should not have NONE type");
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
