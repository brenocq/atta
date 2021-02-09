//--------------------------------------------------
// Atta Ray Tracing Vulkan
// shaderBindingTable.h
// Date: 2020-07-23
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_RT_VK_SHADER_BINDING_TABLE_H
#define ATTA_RT_VK_SHADER_BINDING_TABLE_H

#include "simulator/graphics/vulkan/device.h"
#include "simulator/graphics/vulkan/buffer.h"
#include "deviceProcedures.h"
#include "rayTracingPipeline.h"

namespace atta::rt::vk
{
	class ShaderBindingTable final
	{
		public:
			struct Entry
			{
				uint32_t groupIndex;
				std::vector<unsigned char> inlineData;
			};

			ShaderBindingTable(
				std::shared_ptr<atta::vk::Device> device,
				std::shared_ptr<DeviceProcedures> deviceProcedures,
				std::shared_ptr<RayTracingPipeline> rayTracingPipeline,
				std::shared_ptr<RayTracingProperties> rayTracingProperties,
				const std::vector<Entry>& rayGenPrograms,
				const std::vector<Entry>& missPrograms,
				const std::vector<Entry>& hitGroups);

			~ShaderBindingTable();

			std::shared_ptr<atta::vk::Buffer> getBuffer() const { return _buffer; }

			VkDeviceAddress rayGenDeviceAddress() const { return _buffer->getDeviceAddress() + rayGenOffset(); }
			VkDeviceAddress missDeviceAddress() const { return _buffer->getDeviceAddress() + missOffset(); }
			VkDeviceAddress hitGroupDeviceAddress() const { return _buffer->getDeviceAddress() + hitGroupOffset(); }

			size_t rayGenOffset() const { return _rayGenOffset; }
			size_t missOffset() const { return _missOffset; }
			size_t hitGroupOffset() const { return _hitGroupOffset; }

			size_t rayGenEntrySize() const { return _rayGenEntrySize; }
			size_t missEntrySize() const { return _missEntrySize; }
			size_t hitGroupEntrySize() const { return _hitGroupEntrySize; }

			size_t rayGenSize() const { return _rayGenSize; }
			size_t missSize() const { return _missSize; }
			size_t hitGroupSize() const { return _hitGroupSize; }

		private:
			const size_t _rayGenEntrySize;
			const size_t _missEntrySize;
			const size_t _hitGroupEntrySize;

			const size_t _rayGenOffset;
			const size_t _missOffset;
			const size_t _hitGroupOffset;

			const size_t _rayGenSize;
			const size_t _missSize;
			const size_t _hitGroupSize;

			std::shared_ptr<atta::vk::Device> _device;
			std::shared_ptr<atta::vk::Buffer> _buffer;
	};
}

#endif// ATTA_RT_VK_SHADER_BINDING_TABLE_H
