//--------------------------------------------------
// Atta Ray Tracing CPU
// perspectiveCamera.h
// Date: 2021-02-09
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_RT_CPU_PERSPECTIVE_CAMERA_H
#define ATTA_RT_CPU_PERSPECTIVE_CAMERA_H

#include "camera.h"

namespace atta::rt::cpu
{
	class PerspectiveCamera : public Camera
	{
		public:
			struct CreateInfo
			{
				float width;
				float height;
				mat4 viewMatrix;
				mat4 projMatrix;
			};

			PerspectiveCamera(CreateInfo info);

			CameraRay generateRay(vec2 xy);

		private:
			mat4 _projMatrix;
	};
};
#endif// ATTA_RT_CPU_PERSPECTIVE_CAMERA_H
