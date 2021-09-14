//--------------------------------------------------
// Atta Graphics System
// perspectiveCamera.cpp
// Date: 2021-09-14
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/graphicsSystem/cameras/perspectiveCamera.h>

namespace atta
{
	PerspectiveCamera::PerspectiveCamera(CreateInfo info):
		_near(info.near), _far(info.far), _fov(info.fov)
	{
		_position = info.position;
		_front = normalize(info.lookAt-info.position);
		_up = normalize(info.up);
		_left = cross(_front, _up);
		_up = cross(_front, _left);

		_left.normalize();
		_up.normalize();

		_ratio = info.ratio;
	}

	mat4 PerspectiveCamera::getView() const
	{
		return lookAt(_position, _position+_front, _up);
	}

	mat4 PerspectiveCamera::getProj() const
	{
		return perspective(_fov, _ratio, _near, _far);
	}
}
