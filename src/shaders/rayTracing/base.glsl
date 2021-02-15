//--------------------------------------------------
// Atta Ray Tracing Vulkan GLSL
// random.glsl
// Date: 2021-02-13
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef BASE_GLSL
#define BASE_GLSL

const float piOver4 = 0.78539816339;
const float piOver2 = 1.57079632679;
const float invPi   = 0.318309886181f;

bool isBlack(vec3 v)
{
	return v.x==0 && v.y==0 && v.z==0;
}

float maxComponentValue(vec3 v)
{
	if(v.x >= v.y && v.x >= v.z)
		return v.x;
	if(v.y >= v.z)
		return v.y;
	return v.z;
}

#endif// BASE_GLSL
