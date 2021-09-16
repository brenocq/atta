//--------------------------------------------------
// Atta Graphics System
// camera.cpp
// Date: 2021-09-14
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/graphicsSystem/cameras/camera.h>

namespace atta
{
	void Camera::setFront(const vec3& front)
	{
		_front = normalize(front);
		_left = cross(_up, _front);
		_up = cross(_front, _left);

		_left.normalize();
		_up.normalize();
	}
}