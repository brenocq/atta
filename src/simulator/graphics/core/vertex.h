//--------------------------------------------------
// Atta Graphics
// vertex.h
// Date: 2020-07-06
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_GRAPHICS_VERTEX_H
#define ATTA_GRAPHICS_VERTEX_H

#include "simulator/math/vector.h"
#include "simulator/graphics/vulkan/vulkan.h"
#include <array>

namespace atta
{
	struct Vertex 
	{
		vec3 pos;
		vec3 normal;
		vec2 texCoord;
		int32_t materialIndex;

		static VkVertexInputBindingDescription getBindingDescription() 
		{
			VkVertexInputBindingDescription bindingDescription{};
			bindingDescription.binding = 0;
			bindingDescription.stride = sizeof(Vertex);
			bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

			return bindingDescription;
		}

		static std::array<VkVertexInputAttributeDescription, 4> getAttributeDescriptions() 
		{
			std::array<VkVertexInputAttributeDescription, 4> attributeDescriptions{};

			attributeDescriptions[0].binding = 0;
			attributeDescriptions[0].location = 0;
			attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
			attributeDescriptions[0].offset = offsetof(Vertex, pos);

			attributeDescriptions[1].binding = 0;
			attributeDescriptions[1].location = 1;
			attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
			attributeDescriptions[1].offset = offsetof(Vertex, normal);

			attributeDescriptions[2].binding = 0;
			attributeDescriptions[2].location = 2;
			attributeDescriptions[2].format = VK_FORMAT_R32G32_SFLOAT;
			attributeDescriptions[2].offset = offsetof(Vertex, texCoord);

			attributeDescriptions[3].binding = 0;
			attributeDescriptions[3].location = 3;
			attributeDescriptions[3].format = VK_FORMAT_R32_SINT;
			attributeDescriptions[3].offset = offsetof(Vertex, materialIndex);

			return attributeDescriptions;
		}

		bool operator==(const Vertex& other) const {
			return pos == other.pos && normal == other.normal && texCoord == other.texCoord && materialIndex == other.materialIndex;
		}
	};
}

namespace std {
	template<> struct hash<atta::Vertex> {
		size_t operator()(atta::Vertex const& vertex) const {
			return ((hash<atta::vec3>()(vertex.pos) ^
				   (hash<atta::vec3>()(vertex.normal) << 1)) >> 1) ^
				   (hash<atta::vec2>()(vertex.texCoord) << 1) ^
				   (hash<int32_t>()(vertex.materialIndex) << 1);
		}
	};
}
#endif// ATTA_GRAPHICS_VERTEX_H
