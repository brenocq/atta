//--------------------------------------------------
// Atta Graphics 2D Renderer 
// uniformBuffer2D.h
// Date: 2021-02-10
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_UNIFORM_BUFFER_2D_H
#define ATTA_UNIFORM_BUFFER_2D_H

#include <atta/graphics/vulkan/device.h>
#include <atta/graphics/vulkan/buffer.h>
#include <atta/math/math.h>

namespace atta
{
	class UniformBuffer2D : public vk::Buffer
	{
		public:
			struct Data
			{
				mat4 viewMatrix;
				mat4 orthoMatrix;
			};

			UniformBuffer2D(std::shared_ptr<vk::Device> device);
			~UniformBuffer2D();

			void setValue(Data data);
			Data getValue() const { return _data; }
			
		private:
			Data _data;
	};
}

#endif// ATTA_UNIFORM_BUFFER_2D_H
