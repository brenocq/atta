//--------------------------------------------------
// Atta Ray Tracing CPU
// camera.cpp
// Date: 2021-02-09
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/graphics/renderers/rayTracing/rayTracingCPU/cameras/camera.h>

namespace atta::rt::cpu
{
	Camera::Camera(CreateInfo info):
		_width(info.width), _height(info.height), _viewMatrix(info.viewMatrix)
	{

	}
}
