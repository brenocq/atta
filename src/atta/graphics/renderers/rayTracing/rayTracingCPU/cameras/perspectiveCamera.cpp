//--------------------------------------------------
// Atta Ray Tracing CPU
// perspectiveCamera.h
// Date: 2021-02-09
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/graphics/renderers/rayTracing/rayTracingCPU/cameras/perspectiveCamera.h>

namespace atta::rt::cpu
{
	PerspectiveCamera::PerspectiveCamera(CreateInfo info):
		Camera({info.width, info.height, info.viewMatrix}), _projMatrix(info.projMatrix)
	{

	}

	CameraRay PerspectiveCamera::generateRay(vec2 xy)
	{
		vec3 target = _projMatrix*transpose(_viewMatrix)*vec4((xy.x-0.5)*2, (xy.y-0.5)*2, 1, 0);

		CameraRay cr;
		cr.r.o = pnt3(vec3(inverse(_viewMatrix).col(3)));
		cr.r.d = normalize(target);

		return cr;
	}
}
