//--------------------------------------------------
// Atta Ray Tracing CPU
// rayTracing.h
// Date: 2020-12-29
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_RT_CPU_RAY_TRACING_H
#define ATTA_RT_CPU_RAY_TRACING_H

#include <string>
#include <vector>

#include "simulator/math/math.h"
#include "simulator/graphics/renderers/renderer.h"
#include "simulator/graphics/vulkan/commandPool.h"
#include "simulator/graphics/vulkan/stagingBuffer.h"
#include "simulator/core/scene.h"

#include "cameras/cameras.h"

namespace atta::rt::cpu
{
	class RayTracing : public Renderer
	{
		public:
			struct CreateInfo
			{
				std::shared_ptr<atta::vk::VulkanCore> vkCore;
				std::shared_ptr<atta::vk::CommandPool> commandPool;
				float width;
				float height;
				std::shared_ptr<Scene> scene;
				mat4 viewMat = atta::lookAt(vec3(2,2,0), vec3(0,0,0), vec3(0,1,0));
				mat4 projMat = atta::perspective(atta::radians(45.0), 1200.0/900, 0.01f, 1000.0f);
			};

			RayTracing(CreateInfo info);
			~RayTracing();

			void render(VkCommandBuffer commandBuffer);
			void updateCameraMatrix(mat4 viewMatrix);

		private:
			std::vector<float> _film;

			std::shared_ptr<Scene> _scene;
			unsigned _samplesPerFrame;
			unsigned _totalNumberOfSamples;

			// Ray Tracing objects
			Camera* _camera;

			// Vulkan objects
			std::shared_ptr<atta::vk::StagingBuffer> _stagingBuffer;
	};
};
#endif// ATTA_RT_CPU_RAY_TRACING_H
