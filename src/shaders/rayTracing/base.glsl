//--------------------------------------------------
// Atta Ray Tracing Vulkan GLSL
// random.glsl
// Date: 2021-02-13
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef BASE_GLSL
#define BASE_GLSL

const float pi   	= 3.14159265358f;
const float piOver4 = 0.78539816339f;
const float piOver2 = 1.57079632679f;
const float invPi   = 0.318309886181f;
const float inv2Pi  = 0.159154943091f;

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
