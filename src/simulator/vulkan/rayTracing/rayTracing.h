//--------------------------------------------------
// Robot Simulator
// rayTracing.h
// Date: 2020-07-15
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
#include "shaderBindingTable.h"

class RayTracing
{
	public:
		// Ray Tracing with swapchain
		RayTracing(Device* device, SwapChain* swapChain, CommandPool* commandPool, std::vector<UniformBuffer*> uniformBuffers, Scene* scene);
		// Offline Ray Tracing
		RayTracing(Device* device, VkExtent2D extent, VkFormat format, CommandPool* commandPool, UniformBuffer* uniformBuffer, Scene* scene);
		// Base constructor
		RayTracing(Device* device, SwapChain* swapChain, VkExtent2D extent, VkFormat format, CommandPool* commandPool, std::vector<UniformBuffer*> uniformBuffer, Scene* scene);
		~RayTracing();

		void createPipeline();
		void deletePipeline();
		void render(VkCommandBuffer commandBuffer, const uint32_t imageIndex=0, bool split=false);
		void recreateTopLevelStructures();
	private:
		void getRTProperties();
		void createAccelerationStructures();
		void createBottomLevelStructures(VkCommandBuffer commandBuffer);
		void createTopLevelStructures(VkCommandBuffer commandBuffer);
		void createOutputImage();

		// Main auxiliary objects
		Device* _device;
		CommandPool* _commandPool;
		SwapChain* _swapChain;
		std::vector<UniformBuffer*> _uniformBuffers;
		Scene* _scene;

		VkPhysicalDeviceRayTracingPropertiesNV _props = {};
		DeviceProcedures* _deviceProcedures;

		// Ray Tracing objects
		std::vector<BottomLevelAccelerationStructure*> _blas;
		std::vector<TopLevelAccelerationStructure*> _tlas;
		RayTracingPipeline* _rayTracingPipeline;
		ShaderBindingTable* _shaderBindingTable;

		// Output images info
		VkExtent2D _imageExtent;
		VkFormat _imageFormat;

		// Output images
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
