//--------------------------------------------------
// Atta Rasterization Renderer GLSL
// light.glsl
// Date: 2021-03-09
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_RAST_LIGHTS_LIGHT_GLSL
#define ATTA_RAST_LIGHTS_LIGHT_GLSL
#include "point.glsl"
#include "distant.glsl"
#include "infinite.glsl"

vec3 Light_sampleLi(Light light, out vec3 wi)
{
	switch(light.type)
	{
		case LIGHT_TYPE_POINT:
			return PointLight_sampleLi(light, wi);
		case LIGHT_TYPE_SPOT:
		case LIGHT_TYPE_DISTANT:
			return DistantLight_sampleLi(light, wi);
		case LIGHT_TYPE_INFINITE:
			return InfiniteLight_sampleLi(light, wi);
		default:
			return vec3(0,0,0);
	}
}
#endif// ATTA_RAST_LIGHTS_LIGHT_GLSL
