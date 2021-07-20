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
#include <atta/graphics/vulkan/instance.h>
#include <atta/graphics/vulkan/debugMessenger.h>
#include <atta/graphics/vulkan/physicalDevice.h>
#include <atta/graphics/vulkan/device.h>
#include <atta/graphics/vulkan/buffer.h>
#include <atta/graphics/vulkan/texture.h>
#include <atta/graphics/core/texture.h>
#include <atta/core/scene.h>

namespace atta::vk
{
	class VulkanCore
	{
		public:
			VulkanCore();
			~VulkanCore();

			//---------- Getters ----------//
			std::shared_ptr<Instance> getInstance() const { return _instance; }
			std::shared_ptr<PhysicalDevice> getPhysicalDevice() const { return _physicalDevice; };
			std::shared_ptr<Device> getDevice() const { return _device; }

			std::shared_ptr<Buffer> getVertexBuffer() const { return _vertexBuffer; }
			std::shared_ptr<Buffer> getIndexBuffer() const { return _indexBuffer; }
			std::shared_ptr<Buffer> getMaterialBuffer() const { return _materialBuffer; }
			std::shared_ptr<Buffer> getObjectInfoBuffer() const { return _objectInfoBuffer; }
			std::shared_ptr<Buffer> getLightBuffer() const { return _lightBuffer; }
			std::shared_ptr<Buffer> getLineBuffer() const { return _lineBuffer; }
			std::shared_ptr<Buffer> getPointBuffer() const { return _pointBuffer; }

			std::vector<std::shared_ptr<vk::Texture>> getTextures() const { return _textures; }
			std::vector<std::shared_ptr<vk::Texture>> getTexturesCubeMap() const { return _texturesCubeMap; }

			void createBuffers(std::shared_ptr<Scene> scene, std::shared_ptr<CommandPool> commandPool);
			void updateBuffers(std::shared_ptr<Scene> scene, std::shared_ptr<CommandPool> commandPool);

		private:
			template <class T>
			std::shared_ptr<Buffer> createBufferMemory(std::shared_ptr<CommandPool> commandPool,
					const VkBufferUsageFlags usage, 
					std::vector<T>& content);

			std::shared_ptr<Instance> _instance;
			std::unique_ptr<DebugMessenger> _debugMessenger;
			std::shared_ptr<PhysicalDevice> _physicalDevice;
			std::shared_ptr<Device> _device;

			// Buffers
			std::shared_ptr<Buffer> _vertexBuffer;
			std::shared_ptr<Buffer> _indexBuffer;
			std::shared_ptr<Buffer> _materialBuffer;
			std::shared_ptr<Buffer> _objectInfoBuffer;
			std::shared_ptr<Buffer> _lightBuffer;

			// Aux/Debug buffers
			std::shared_ptr<Buffer> _lineBuffer;
			std::shared_ptr<Buffer> _pointBuffer;

			// Texture images
			std::vector<std::shared_ptr<vk::Texture>> _textures;
			std::vector<std::shared_ptr<vk::Texture>> _texturesCubeMap;
	};
}

#endif// ATTA_GRAPHICS_VULKAN_CORE_H
