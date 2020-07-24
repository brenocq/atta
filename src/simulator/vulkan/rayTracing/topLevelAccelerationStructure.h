//--------------------------------------------------
// Robot Simulator
// topLevelAcceletationStructure.h
// Date: 22/07/2020
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef TOP_LEVEL_ACCELERATION_STRUCTURE_H
#define TOP_LEVEL_ACCELERATION_STRUCTURE_H

#include <vector>
#include "../../scene.h"
#include "../buffer.h"
#include "accelerationStructure.h"
#include "bottomLevelAccelerationStructure.h"

// Geometry instance, with the layout expected by VK_NV_ray_tracing
struct VkGeometryInstance
{
	// Transform matrix, containing only the top 3 rows
	float transform[12];
	// Instance index
	uint32_t instanceCustomIndex : 24;
	// Visibility mask
	uint32_t mask : 8;
	// Index of the hit group which will be invoked when a ray hits the instance
	uint32_t instanceOffset : 24;
	// Instance flags, such as culling
	uint32_t flags : 8;
	// Opaque handle of the bottom-level acceleration structure
	uint64_t accelerationStructureHandle;
};

class TopLevelAccelerationStructure final : public AccelerationStructure
{
	public:
	TopLevelAccelerationStructure(DeviceProcedures* deviceProcedures, const std::vector<VkGeometryInstance>& geometryInstances, bool allowUpdate);
	~TopLevelAccelerationStructure();

	const std::vector<VkGeometryInstance>& getGeometryInstances() const { return _geometryInstances; }

	void generate(
		VkCommandBuffer commandBuffer,
		Buffer* resultBuffer,
		VkDeviceSize resultOffset,
		Buffer* scratchBuffer,
		VkDeviceSize scratchOffset,
		Buffer* instanceBuffer,
		VkDeviceSize instanceOffset,
		bool updateOnly) const;

	static VkGeometryInstance createGeometryInstance(
		BottomLevelAccelerationStructure* bottomLevelAs,
		const glm::mat4& transform,
		uint32_t instanceId,
		uint32_t hitGroupIndex);

	private:
	std::vector<VkGeometryInstance> _geometryInstances;
};

#endif// TOP_LEVEL_ACCELERATION_STRUCTURE_H
