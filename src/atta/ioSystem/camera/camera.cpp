//--------------------------------------------------
// Atta IO System
// camera.h
// Date: 2021-09-26
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/ioSystem/camera/camera.h>

namespace atta::io
{
	Camera::Camera(CreateInfo info):
		_deviceName(info.deviceName), 
		_pixelFormat(info.pixelFormat), _resolution(info.resolution),
		_fps(info.fps), _debugName(info.debugName)
	{

	}
}
