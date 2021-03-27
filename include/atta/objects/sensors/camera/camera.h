//--------------------------------------------------
// Robot Simulator
// camera.h
// Date: 2020-10-30
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_OBJECTS_SENSORS_CAMERA_CAMERA_H
#define ATTA_OBJECTS_SENSORS_CAMERA_CAMERA_H

#include <string>
#include <vector>
#include <atta/objects/object.h>

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
				std::string name = "Camera";
				vec3 position = {0,0,0};
				vec3 rotation = {0,0,0};
				vec3 scale = {1,1,1};
				float mass = 1.0f;

				RenderingType renderingType = RASTERIZATION;
				unsigned width = 240;
				unsigned height = 240;
				float fov = 24.0f;

				std::vector<std::shared_ptr<Object>> children = {};
			};

			Camera(CreateInfo info);
			~Camera();

			//---------- Getters ----------//
			RenderingType getRenderingType() const { return _renderingType; }
			unsigned getWidth() const { return _width; }
			unsigned getHeight() const { return _height; }
			float getFov() const { return _fov; }
			std::vector<uint8_t> getBuffer() const { return _buffer; }

		private:
			friend class ThreadManager;
			// The Thread Manager changes the camera buffer after rendering is complete
			void setBuffer(std::vector<uint8_t> buffer) { _buffer=buffer; }

			// Camera parameters
			RenderingType _renderingType;
			unsigned _width;
			unsigned _height;
			float _fov;

			// Image buffer
			std::vector<uint8_t> _buffer;
	};
}

#endif// ATTA_OBJECTS_SENSORS_CAMERA_CAMERA_H
