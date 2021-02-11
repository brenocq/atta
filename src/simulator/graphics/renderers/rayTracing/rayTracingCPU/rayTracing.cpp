//--------------------------------------------------
// Atta Ray Tracing CPU
// rayTracing.cpp
// Date: 2020-12-29
// By Breno Cunha Queiroz
//--------------------------------------------------
#include "rayTracing.h"
#include "simulator/helpers/log.h"
#include "simulator/helpers/drawer.h"
#include "simulator/graphics/vulkan/imageMemoryBarrier.h"

namespace atta::rt::cpu
{
	RayTracing::RayTracing(CreateInfo info):
		Renderer({info.vkCore, info.width, info.height, info.viewMat, RENDERER_TYPE_RAY_TRACING_CPU}), 
		_scene(info.scene)
	{
		_film.resize(info.width*info.height*4);// VK_FORMAT_R32G32B32A32_SFLOAT
		_samplesPerFrame = 4;
		_totalNumberOfSamples = 0;

		// Create camera
		_camera = (Camera*)(new PerspectiveCamera({info.width, info.height, info.viewMat, atta::perspective(atta::radians(45.0), 1200.0/900, 0.01f, 1000.0f)}));

		// Staging buffer used to send film data to vulkan image
		_stagingBuffer = std::make_shared<atta::vk::StagingBuffer>(_vkCore->getDevice(), _film.data(), _film.size()*sizeof(_film[0]));

		// Change output image layout to the expected by rendering
		VkImageSubresourceRange subresourceRange;
		subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		subresourceRange.baseMipLevel = 0;
		subresourceRange.levelCount = 1;
		subresourceRange.baseArrayLayer = 0;
		subresourceRange.layerCount = 1;

		VkCommandBuffer commandBuffer = _vkCore->getCommandPool()->beginSingleTimeCommands();
		{
			atta::vk::ImageMemoryBarrier::insert(commandBuffer, _image->handle(), subresourceRange, VK_ACCESS_TRANSFER_WRITE_BIT,
				0, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL);
		}
		_vkCore->getCommandPool()->endSingleTimeCommands(commandBuffer);

		Log::success("rt::cpu::RayTracing", "CPU Raytracing was successfully initialized");
	}

	RayTracing::~RayTracing()
	{
		if(_camera!=nullptr)
		{
			delete _camera;
			_camera = nullptr;
		}
	}

	void RayTracing::render(VkCommandBuffer commandBuffer)
	{
		// Trace rays for each pixel
		for(int x=0; x<_extent.width; x++)
		{
			for(int y=0; y<_extent.height; y++)
			{
				float xNorm = float(x)/_extent.width;
				float yNorm = float(y)/_extent.height;
				CameraRay cr = _camera->generateRay(vec2(xNorm, yNorm));
				unsigned pixelPos = 4*(y*_extent.width+x);
				_film[pixelPos+0] = cr.r.d.x;
				_film[pixelPos+1] = cr.r.d.y;
				_film[pixelPos+2] = 0.0f;
				_film[pixelPos+3] = 1.0f;

				if(x%100==0 && y%100==0)
					Drawer::addLine({cr.r.o, cr.r.o+cr.r.d, vec3(xNorm,yNorm,0), vec3(xNorm,yNorm,0)});
			}
		}

		//---------- Copy film to vulkan image ----------//
		//_stagingBuffer->mapFromData(_vkCore->getDevice(), _film.data(), _film.size()*sizeof(_film[0]));
		//
		//VkImageSubresourceRange subresourceRange;
		//subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		//subresourceRange.baseMipLevel = 0;
		//subresourceRange.levelCount = 1;
		//subresourceRange.baseArrayLayer = 0;
		//subresourceRange.layerCount = 1;

		//VkBufferImageCopy region{};
		//region.bufferOffset = 0;
		//region.bufferRowLength = 0;
		//region.bufferImageHeight = 0;

		//region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		//region.imageSubresource.mipLevel = 0;
		//region.imageSubresource.baseArrayLayer = 0;
		//region.imageSubresource.layerCount = 1;

		//region.imageOffset = {0, 0, 0};
		//region.imageExtent = {
		//	_extent.width,
		//	_extent.height,
		//	1
		//};

		//// Change output image format to the transfer dst
		//atta::vk::ImageMemoryBarrier::insert(commandBuffer, _image->handle(), subresourceRange, VK_ACCESS_TRANSFER_WRITE_BIT,
		//	0, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);

		//vkCmdCopyBufferToImage(
		//	commandBuffer,
		//	_stagingBuffer->handle(),
		//	_image->handle(),
		//	VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
		//	1,
		//	&region
		//);

		//// Change output image format to the expected by the workerGui thread
		//atta::vk::ImageMemoryBarrier::insert(commandBuffer, _image->handle(), subresourceRange, VK_ACCESS_TRANSFER_WRITE_BIT,
		//	0, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);
	}

	void RayTracing::updateCameraMatrix(mat4 viewMatrix)
	{
		_viewMatrix = viewMatrix;
		_totalNumberOfSamples = 0;
		_camera->setViewMatrix(viewMatrix);
	}
}
