//--------------------------------------------------
// Atta Ray Tracing CPU
// camera.h
// Date: 2021-02-09
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_RT_CPU_CAMERA_H
#define ATTA_RT_CPU_CAMERA_H

#include "simulator/math/math.h"

namespace atta::rt::cpu
{
	struct CameraRay
	{
		ray r;
	};

	class Camera
	{
		public:
			struct CreateInfo
			{
				float width;
				float height;
				mat4 viewMatrix;
			};

			Camera(CreateInfo info);

			virtual CameraRay generateRay(vec2 xy) = 0;

		private:
			float _width;
			float _height;
			mat4 _viewMatrix;
	};
};
#endif// ATTA_RT_CPU_CAMERA_H
