//--------------------------------------------------
// Atta Graphics System
// vulkanRenderer.h
// Date: 2021-08-30
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_GRAPHICS_SYSTEM_RENDERER_APIS_VULKAN_VULKAN_RENDERER_H
#define ATTA_GRAPHICS_SYSTEM_RENDERER_APIS_VULKAN_VULKAN_RENDERER_H
#include <atta/graphicsSystem/rendererAPIs/rendererAPI.h>

namespace atta
{
	class VulkanRenderer final : public RendererAPI
	{
	public:
		VulkanRenderer();
		~VulkanRenderer();

		void beginFrame() override;
		void endFrame() override;
	};
}

#endif// ATTA_GRAPHICS_SYSTEM_RENDERER_APIS_VULKAN_VULKAN_RENDERER_H
