//--------------------------------------------------
// Robot Simulator
// helpers.h
// Date: 24/06/2020
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef HELPERS_H
#define HELPERS_H
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <vector>
#include <optional>

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

struct UniformBufferObject
{
	glm::mat4 modelView;
	glm::mat4 projection;
	glm::mat4 modelViewInverse;
	glm::mat4 projectionInverse;
	float aperture;
	float focusDistance;
	uint32_t totalNumberOfSamples;
	uint32_t numberOfSamples;
	uint32_t numberOfBounces;
	uint32_t randomSeed;
	uint32_t gammaCorrection; // bool
	uint32_t hasSky; // bool
};


struct ObjectInfo
{
	glm::mat4 modelMatrix;
	glm::vec3 color;
};

// TLAS instances (Ray tracing)
struct InstanceInfo
{
	glm::mat4 transform{1};    // Position of the instance
	glm::mat4 transformIT{1};  // Inverse transpose
	glm::vec4 diffuse;  // Inverse transpose
};

#endif// HELPERS_H
