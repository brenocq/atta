//--------------------------------------------------
// Atta Graphics System
// perspectiveCamera.h
// Date: 2021-09-14
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_GRAPHICS_SYSTEM_CAMERAS_PERSPECTIVE_CAMERA_H
#define ATTA_GRAPHICS_SYSTEM_CAMERAS_PERSPECTIVE_CAMERA_H
#include <atta/graphicsSystem/cameras/camera.h>

namespace atta
{
	class PerspectiveCamera : public Camera
	{
	public:
		struct CreateInfo
		{
			vec3 position = {-3.0f, 0.0f, 0.0f};
			vec3 lookAt = {0.0f, 0.0f, 0.0f};
			vec3 up = {0.0f, 0.0f, 1.0f};
			float near = 0.001f;
			float far = 1000.0f;
			float fov = 45.0f;
			float ratio = 1.0f;
		};

		PerspectiveCamera(CreateInfo info);
		mat4 getView() const override;
		mat4 getProj() const override;

		void move() override;

	private:
		float _near;
		float _far;
		float _fov;

		float _speed;
	};
}

#endif// ATTA_GRAPHICS_SYSTEM_CAMERAS_PERSPECTIVE_CAMERA_H
