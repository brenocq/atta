//--------------------------------------------------
// Robot Simulator
// helpers.h
// Date: 2020-06-24
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef HELPERS_H
#define HELPERS_H
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vector>
#include <optional>
#include "simulator/math/math.h"

struct QueueFamilyIndices {
    std::optional<uint32_t> graphicsFamily;
    std::optional<uint32_t> presentFamily;

    bool isComplete() {
        return graphicsFamily.has_value() && presentFamily.has_value();
    }
};

struct SwapChainSupportDetails
{
	VkSurfaceCapabilitiesKHR capabilities;
	std::vector<VkSurfaceFormatKHR> formats;
	std::vector<VkPresentModeKHR> presentModes;
};

// TLAS instances (Ray tracing)
//struct InstanceInfo
//{
//	atta::mat4 transform{1};    // Position of the instance
//	atta::mat4 transformIT{1};  // Inverse transpose
//	atta::vec4 diffuse;  // Inverse transpose
//};

#endif// HELPERS_H
