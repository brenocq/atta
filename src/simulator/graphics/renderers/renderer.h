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

namespace atta
{
	class Renderer
	{
		public:
			Renderer();

			virtual void render() = 0;

		protected:
			std::shared_ptr<vk::Image> _image;
			std::shared_ptr<vk::ImageView> _imageView;
	};
}
#endif// ATTA_GRAPHICS_RENDERER_H
