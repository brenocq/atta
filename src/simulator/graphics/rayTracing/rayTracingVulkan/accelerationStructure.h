//--------------------------------------------------
// Robot Simulator
// acceletationStructure.h
// Date: 2020-07-16
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ACCELERATION_STRUCTURE_H
#define ACCELERATION_STRUCTURE_H

#include "../device.h"
#include "../buffer.h"
#include "deviceProcedures.h"

class AccelerationStructure
{
	public:
		struct MemoryRequirements
		{
			VkMemoryRequirements result;
			VkMemoryRequirements build;
			VkMemoryRequirements update;
		};

		virtual ~AccelerationStructure();

		std::shared_ptr<Device> getDevice() const { return _device; }
		DeviceProcedures* getDeviceProcedures() const { return _deviceProcedures; }
		MemoryRequirements getMemoryRequirements() const;
		VkAccelerationStructureNV handle() const { return _accelerationStructure; }
		
		static void memoryBarrier(VkCommandBuffer commandBuffer);
	protected:
		AccelerationStructure(DeviceProcedures* deviceProcedures, const VkAccelerationStructureCreateInfoNV& createInfo);

		DeviceProcedures* _deviceProcedures;
		const bool _allowUpdate;
	private:
		std::shared_ptr<Device> _device;
		VkAccelerationStructureNV _accelerationStructure;
};

#endif// ACCELERATION_STRUCTURE_H
