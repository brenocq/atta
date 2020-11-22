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
#include "simulator/object.h"
#include "simulator/vulkan/application.h"

class Camera : public Object
{
	public:
		enum CameraRenderingType
		{
			RAY_TRACING,
			RASTERIZATION
		};

		struct CameraInfo
		{
			CameraRenderingType renderingType = RAY_TRACING;
			unsigned int width = 200;
			unsigned int height = 200;
			float fov = 30.0f;
		};

		Camera(std::string name, glm::vec3 position = {0,0,0}, glm::vec3 rotation = {0,0,0}, CameraInfo cameraInfo={RAY_TRACING, 200, 200, 30.0f});
		~Camera();

		void takePicture();

		//---------- Getters and Setters ----------//
		void setRayTracingPipelineIndex(int index) { _rayTracingPipelineIndex = index; }
		int getRayTracingPipelineIndex() const { return _rayTracingPipelineIndex; }
		void setRasterizationPipelineIndex(int index) { _rasterizationPipelineIndex = index; }
		int getRasterizationPipelineIndex() const { return _rasterizationPipelineIndex; }
		CameraRenderingType getRenderingType() const { return _info.renderingType; }
		CameraInfo getCameraInfo() const { return _info; }

	private:
		// Camera parameters
		CameraInfo _info;

		// Simulation parameters
		int _rayTracingPipelineIndex;
		int _rasterizationPipelineIndex;

		// Image buffer
		std::vector<uint8_t> _buffer;
};

#endif// CAMERA_H
