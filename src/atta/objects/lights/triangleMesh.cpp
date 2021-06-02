//--------------------------------------------------
// Robot Simulator
// triangleMeshLight.cpp
// Date: 2021-06-02
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/objects/lights/triangleMesh.h>

namespace atta
{
	TriangleMeshLight::TriangleMeshLight(CreateInfo info):
		Object({info.name, {0,0,0}, {0,0,0}, {1,1,1}, 0}),
		_radiance(info.radiance), _vertices(info.vertices)
	{
		Object::setType("TriangleMeshLight");
		_isLight = true;
	}

	TriangleMeshLight::~TriangleMeshLight()
	{
	}
}
