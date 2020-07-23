//--------------------------------------------------
// Robot Simulator
// rayTracing.h
// Date: 15/07/2020
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef RAY_TRACING_H
#define RAY_TRACING_H

#include <iostream>
#include <string.h>
#include <vector>

#include "defines.h"
#include "../device.h"
#include "../commandPool.h"
#include "../../scene.h"
#include "deviceProcedures.h"
#include "bottomLevelAccelerationStructure.h"
#include "topLevelAccelerationStructure.h"

class RayTracing
{
	public:
	RayTracing(Device* device, CommandPool* commandPool, Scene* scene);
	~RayTracing();

	private:
	void getRTProperties();
	void createAccelerationStructures();
	void createBottomLevelStructures(VkCommandBuffer commandBuffer);
	void createTopLevelStructures(VkCommandBuffer commandBuffer);

	Device* _device;
	CommandPool* _commandPool;
	Scene* _scene;

	VkPhysicalDeviceRayTracingPropertiesNV _props = {};
	DeviceProcedures* _deviceProcedures;

	std::vector<BottomLevelAccelerationStructure> _blas;
	std::vector<TopLevelAccelerationStructure> _tlas;
	
	Buffer* _bottomBuffer;
	Buffer* _bottomScratchBuffer;
	Buffer* _topBuffer;
	Buffer* _topScratchBuffer;
	Buffer* _instancesBuffer;
};

#endif// RAY_TRACING_H
