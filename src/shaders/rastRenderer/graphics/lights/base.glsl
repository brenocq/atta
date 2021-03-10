//--------------------------------------------------
// Atta Rasterization Renderer GLSL
// base.glsl
// Date: 2021-03-08
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_RAST_LIGHTS_BASE_GLSL
#define ATTA_RAST_LIGHTS_BASE_GLSL

float sphericalTheta(vec3 v)
{
	return acos(clamp(v.z, -1, 1));
}

float sphericalPhi(vec3 v)
{
	float p = atan(v.y, v.x);
	return (p<0) ? (p+2*pi) : p;
}

#endif// ATTA_RAST_LIGHTS_BASE_GLSL
