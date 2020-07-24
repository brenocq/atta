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
#include "../swapChain.h"
#include "../uniformBuffer.h"
#include "../image.h"
#include "../imageView.h"
#include "../../scene.h"
#include "deviceProcedures.h"
#include "bottomLevelAccelerationStructure.h"
#include "topLevelAccelerationStructure.h"
#include "rayTracingPipeline.h"

class RayTracing
{
	public:
	RayTracing(Device* device, SwapChain* swapChain, CommandPool* commandPool, std::vector<UniformBuffer*> uniformBuffers, Scene* scene);
	~RayTracing();

	void createSwapChain();
	void deleteSwapChain();
	private:
	void getRTProperties();
	void createAccelerationStructures();
	void createBottomLevelStructures(VkCommandBuffer commandBuffer);
	void createTopLevelStructures(VkCommandBuffer commandBuffer);
	void createOutputImage();

	Device* _device;
	CommandPool* _commandPool;
	SwapChain* _swapChain;
	std::vector<UniformBuffer*> _uniformBuffers;
	Scene* _scene;

	VkPhysicalDeviceRayTracingPropertiesNV _props = {};
	DeviceProcedures* _deviceProcedures;

	std::vector<BottomLevelAccelerationStructure> _blas;
	std::vector<TopLevelAccelerationStructure> _tlas;
	RayTracingPipeline* _rayTracingPipeline;

	Image* _accumulationImage;
	ImageView* _accumulationImageView;
	Image* _outputImage;
	ImageView* _outputImageView;
	
	Buffer* _bottomBuffer;
	Buffer* _bottomScratchBuffer;
	Buffer* _topBuffer;
	Buffer* _topScratchBuffer;
	Buffer* _instancesBuffer;
};

#endif// RAY_TRACING_H
