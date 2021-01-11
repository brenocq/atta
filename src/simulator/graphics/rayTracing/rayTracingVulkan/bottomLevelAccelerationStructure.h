//--------------------------------------------------
// Robot Simulator
// bottomLevelAcceletationStructure.h
// Date: 2020-07-16
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef BOTTOM_LEVEL_ACCELERATION_STRUCTURE_H
#define BOTTOM_LEVEL_ACCELERATION_STRUCTURE_H

#include "accelerationStructure.h"
#include <vector>
#include "../../scene.h"
#include "simulator/graphics/vulkan/buffer.h"

class BottomLevelAccelerationStructure final : public AccelerationStructure
{
	public:
	BottomLevelAccelerationStructure(DeviceProcedures* deviceProcedures, const std::vector<VkGeometryNV>& geometries, bool allowUpdate);
	~BottomLevelAccelerationStructure();

	void generate(
		VkCommandBuffer commandBuffer,
		Buffer* resultBuffer,
		VkDeviceSize resultOffset,
		Buffer* scratchBuffer,
		VkDeviceSize scratchOffset,
		bool updateOnly) const;

	static VkGeometryNV createGeometry(
		Scene* scene, 
		uint32_t vertexOffset, uint32_t vertexCount,
		uint32_t indexOffset, uint32_t indexCount,
		bool isOpaque);

	static VkGeometryNV createGeometryAabb(
		Scene* scene,
		uint32_t aabbOffset,
		uint32_t aabbCount,
		bool isOpaque);

	private:

	std::vector<VkGeometryNV> _geometries;
};

#endif// BOTTOM_LEVEL_ACCELERATION_STRUCTURE_H
