//--------------------------------------------------
// Atta Ray Tracing Vulkan GLSL
// point.glsl
// Date: 2021-02-13
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef LIGHTS_POINT_GLSL
#define LIGHTS_POINT_GLSL
#include "base.glsl"

vec3 PointLight_sampleLi(
		Light light, VisibilityPoint vp, vec2 u, 
		out vec3 wi, out float pdf, out VisibilityTester vis, 
		vec3 I)
{
	vec3 pLight = (light.lightToWorld * vec4(0,0,0,1)).xyz;
	
	wi = normalize(pLight - vp.point);
	pdf = 1.f;
	vis.p0.point = vp.point;
	vis.p0.wo = vp.wo;
	vis.p0.n = vp.n;
	vis.p1.point = pLight;

	vec3 p = pLight-vp.point;
	return I/dot(p,p);
}

vec3 PointLight_power(vec3 I)
{
	return 4 * 3.1415926535f * I;
}

uint PointLight_flags()
{
	return LIGHT_FLAG_DELTA_POSITION;
}

#endif// LIGHTS_POINT_GLSL
