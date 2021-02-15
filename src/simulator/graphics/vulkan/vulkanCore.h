//--------------------------------------------------
// Atta Graphics
// vulkanCore.h
// Date: 2021-01-11
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_GRAPHICS_VULKAN_CORE_H
#define ATTA_GRAPHICS_VULKAN_CORE_H

#include <vector>
#include <memory>
#include "simulator/graphics/vulkan/instance.h"
#include "simulator/graphics/vulkan/debugMessenger.h"
#include "simulator/graphics/vulkan/physicalDevice.h"
#include "simulator/graphics/vulkan/device.h"
#include "simulator/graphics/vulkan/buffer.h"
#include "simulator/graphics/vulkan/texture.h"
#include "simulator/graphics/core/texture.h"
#include "simulator/core/scene.h"

namespace atta::vk
{
	struct MeshOffset
	{
		unsigned vertexOffset;
		unsigned indexOffset;
	};

	struct InstanceInfo
	{
		mat4 transform;
	};

	class VulkanCore
	{
		public:
			VulkanCore();
			~VulkanCore();

			//---------- Getters ----------//
			std::shared_ptr<Instance> getInstance() const { return _instance; }
			std::shared_ptr<PhysicalDevice> getPhysicalDevice() const { return _physicalDevice; };
			std::shared_ptr<CommandPool> getCommandPool() const { return _commandPool; };
			std::shared_ptr<Device> getDevice() const { return _device; }

			std::shared_ptr<Buffer> getVertexBuffer() const { return _vertexBuffer; }
			std::shared_ptr<Buffer> getIndexBuffer() const { return _indexBuffer; }
			std::shared_ptr<Buffer> getMaterialBuffer() const { return _materialBuffer; }
			std::shared_ptr<Buffer> getObjectInfoBuffer() const { return _objectInfoBuffer; }
			std::shared_ptr<Buffer> getLightBuffer() const { return _lightBuffer; }

			std::vector<std::shared_ptr<vk::Texture>> getTextures() const { return _textures; }

			void createBuffers(std::shared_ptr<Scene> scene);
			void updateBuffers(std::shared_ptr<Scene> scene);

		private:
			template <class T>
			std::shared_ptr<Buffer> createBufferMemory(
					const VkBufferUsageFlags usage, 
					std::vector<T>& content);

			std::shared_ptr<Instance> _instance;
			std::unique_ptr<DebugMessenger> _debugMessenger;
			std::shared_ptr<PhysicalDevice> _physicalDevice;
			std::shared_ptr<Device> _device;
			std::shared_ptr<CommandPool> _commandPool;

			// Buffers
			std::shared_ptr<Buffer> _vertexBuffer;
			std::shared_ptr<Buffer> _indexBuffer;
			std::shared_ptr<Buffer> _materialBuffer;
			std::shared_ptr<Buffer> _objectInfoBuffer;
			std::shared_ptr<Buffer> _lightBuffer;

			// Texture images
			std::vector<std::shared_ptr<vk::Texture>> _textures;
	};
}

#endif// ATTA_GRAPHICS_VULKAN_CORE_H
