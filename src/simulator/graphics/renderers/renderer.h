//--------------------------------------------------
// Atta Graphics
// renderer.h
// Date: 2021-01-11
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_GRAPHICS_RENDERER_H
#define ATTA_GRAPHICS_RENDERER_H

#include <memory>
#include <vector>

#include "simulator/graphics/vulkan/image.h"
#include "simulator/graphics/vulkan/imageView.h"
#include "simulator/graphics/vulkan/vulkanCore.h"
#include "simulator/math/matrix.h"

namespace atta
{
	enum RendererType
	{
		RENDERER_TYPE_RASTERIZATION = 0,
		RENDERER_TYPE_RAY_TRACING_VULKAN,
		RENDERER_TYPE_RAY_TRACING_CPU,
		RENDERER_TYPE_2D
	};

	class Renderer
	{
		public:
			struct CreateInfo
			{
				std::shared_ptr<vk::VulkanCore> vkCore;
				float width;
				float height;
				mat4 viewMatrix;
				RendererType type;
			};

			Renderer(CreateInfo info);

			virtual void render(VkCommandBuffer commandBuffer) = 0;
			virtual void updateCameraMatrix(mat4 viewMatrix) = 0;

			//---------- Getters ----------//
			std::shared_ptr<vk::Image> getImage() const { return _image; }
			std::shared_ptr<vk::ImageView> getImageView() const { return _imageView; }
			mat4 getViewMatrix() const { return _viewMatrix; }
			RendererType getType() const { return _type; }

		protected:
			void createOutputImage();

			RendererType _type;

			std::shared_ptr<vk::VulkanCore> _vkCore;
			VkExtent2D _extent;
			mat4 _viewMatrix;

			std::shared_ptr<vk::Image> _image;
			std::shared_ptr<vk::ImageView> _imageView;
	};
}
#endif// ATTA_GRAPHICS_RENDERER_H
