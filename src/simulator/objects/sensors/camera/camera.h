//--------------------------------------------------
// Robot Simulator
// camera.h
// Date: 2020-10-30
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef CAMERA_H
#define CAMERA_H

#include <string>
#include <vector>
#include "simulator/objects/object.h"

namespace atta
{
	class Camera : public Object
	{
		public:
			enum RenderingType
			{
				RAY_TRACING,
				RASTERIZATION
			};

			struct CreateInfo
			{
				std::string name = "Box";
				vec3 position = {0,0,0};
				vec3 rotation = {0,0,0};
				vec3 scale = {1,1,1};
				float mass = 1.0f;

				RenderingType renderingType = RAY_TRACING;
				unsigned width = 200;
				unsigned height = 200;
				float fov = 30.0f;
			};

			Camera(CreateInfo info);
			~Camera();

			void takePicture();

			//---------- Getters ----------//
			int getRayTracingPipelineIndex() const { return _rayTracingPipelineIndex; }
			int getRasterizationPipelineIndex() const { return _rasterizationPipelineIndex; }
			int getUniformBufferIndex() const { return _uniformBufferIndex; }
			RenderingType getRenderingType() const { return _renderingType; }
			std::vector<uint8_t> getCameraBuffer() const { return _buffer; }

			//---------- Setters ----------//
			void setRayTracingPipelineIndex(int index) { _rayTracingPipelineIndex = index; }
			void setRasterizationPipelineIndex(int index) { _rasterizationPipelineIndex = index; }
			void setUniformBufferIndex(int index) { _uniformBufferIndex = index; }
			void setCameraBuffer(std::vector<uint8_t> buffer) { _buffer=buffer; }

		private:
			// Camera parameters
			RenderingType _renderingType;
			unsigned _width;
			unsigned _height;
			float _fov;

			// Simulation parameters
			int _rayTracingPipelineIndex;
			int _rasterizationPipelineIndex;
			int _uniformBufferIndex;

			// Image buffer
			std::vector<uint8_t> _buffer;
	};
}

#endif// CAMERA_H
