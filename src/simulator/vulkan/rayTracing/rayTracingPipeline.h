//--------------------------------------------------
// Robot Simulator
// rayTracingPipeline.h
// Date: 2020-07-22
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef RAY_TRACING_PIPELINE_H
#define RAY_TRACING_PIPELINE_H

#include <vector>
#include "../../scene.h"
#include "../uniformBuffer.h"
#include "../imageView.h"
#include "../pipeline/pipelineLayout.h"
#include "../swapChain.h"
#include "../device.h"
#include "../descriptorSetManager.h"
#include "deviceProcedures.h"
#include "topLevelAccelerationStructure.h"

class RayTracingPipeline final
{
	public:
	RayTracingPipeline(
		Device* device,
		DeviceProcedures* deviceProcedures,
		uint32_t qtyImages,
		TopLevelAccelerationStructure* accelerationStructure,
		ImageView* accumulationImageView,
		ImageView* outputImageView,
		std::vector<UniformBuffer*> uniformBuffers,
		Scene* scene);
	~RayTracingPipeline();

	uint32_t getRayGenShaderIndex() const { return _rayGenIndex; }
	uint32_t getMissShaderIndex() const { return _missIndex; }
	uint32_t getTriangleHitGroupIndex() const { return _triangleHitGroupIndex; }
	uint32_t getProceduralHitGroupIndex() const { return _proceduralHitGroupIndex; }

	VkDescriptorSet getDescriptorSet(uint32_t index) const;
	PipelineLayout* getPipelineLayout() const { return _pipelineLayout; }
	VkPipeline handle() const { return _pipeline; }

	private:
	VkPipeline _pipeline;

	DescriptorSetManager* _descriptorSetManager;
	PipelineLayout* _pipelineLayout;
	Device* _device;
	uint32_t _qtyImages;
	Scene* _scene;

	uint32_t _rayGenIndex;
	uint32_t _missIndex;
	uint32_t _triangleHitGroupIndex;
	uint32_t _proceduralHitGroupIndex;
};

#endif// RAY_TRACING_PIPELINE_H
