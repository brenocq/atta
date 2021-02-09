//--------------------------------------------------
// Atta Ray Tracing Vulkan
// rayTracingProperties.h
// Date: 2021-02-06
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_RT_VK_RAY_TRACING_PROPERTIES_H
#define ATTA_RT_VK_RAY_TRACING_PROPERTIES_H
#include "simulator/graphics/vulkan/device.h"

namespace atta::rt::vk
{
	class RayTracingProperties final
	{
		public:
			RayTracingProperties(std::shared_ptr<atta::vk::Device> device);

			std::shared_ptr<atta::vk::Device> getDevice() const { return _device; }

			uint32_t maxDescriptorSetAccelerationStructures() const { return _accelProps.maxDescriptorSetAccelerationStructures; }
			uint64_t maxGeometryCount() const { return _accelProps.maxGeometryCount; }
			uint64_t maxInstanceCount() const { return _accelProps.maxInstanceCount; }
			uint64_t maxPrimitiveCount() const { return _accelProps.maxPrimitiveCount; }
			uint32_t maxRayRecursionDepth() const { return _pipelineProps.maxRayRecursionDepth; }
			uint32_t maxShaderGroupStride() const { return _pipelineProps.maxShaderGroupStride; }
			uint32_t minAccelerationStructureScratchOffsetAlignment() const { return _accelProps.minAccelerationStructureScratchOffsetAlignment; }
			uint32_t shaderGroupBaseAlignment() const { return _pipelineProps.shaderGroupBaseAlignment; }
			uint32_t shaderGroupHandleCaptureReplaySize() const { return _pipelineProps.shaderGroupHandleCaptureReplaySize; }
			uint32_t shaderGroupHandleSize() const { return _pipelineProps.shaderGroupHandleSize; }

		private:

			std::shared_ptr<atta::vk::Device> _device;
			VkPhysicalDeviceAccelerationStructurePropertiesKHR _accelProps;
			VkPhysicalDeviceRayTracingPipelinePropertiesKHR _pipelineProps;
	};
}

#endif// ATTA_RT_VK_RAY_TRACING_PROPERTIES_H
