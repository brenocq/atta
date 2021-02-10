//--------------------------------------------------
// Atta Ray Tracing CPU
// perspectiveCamera.h
// Date: 2021-02-09
// By Breno Cunha Queiroz
//--------------------------------------------------
#include "perspectiveCamera.h"

namespace atta::rt::cpu
{
	PerspectiveCamera::PerspectiveCamera(CreateInfo info):
		Camera({info.width, info.height, info.viewMatrix}), _projMatrix(info.projMatrix)
	{

	}

	CameraRay PerspectiveCamera::generateRay(vec2 xy)
	{
		CameraRay cr;
		cr.r.o = pnt3(0,0,0);
		cr.r.d = vec3(0,0,1);

		return cr;
	}
}
