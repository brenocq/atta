//--------------------------------------------------
// Atta Ray Tracing Vulkan GLSL
// distant.glsl
// Date: 2021-02-13
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef LIGHTS_DISTANT_GLSL
#define LIGHTS_DISTANT_GLSL
#include "base.glsl"

// Current assumption: The scene sphere diameter is less than 10000 meters

float DistantLight_falloff(Light light, vec3 w, float cosTotalWidth, float cosFalloffStart);

vec3 DistantLight_sampleLi(
		Light light, VisibilityPoint vp, vec2 u, 
		out vec3 wi, out float pdf, out VisibilityTester vis, 
		vec3 L, vec3 wLight)
{
	wi = wLight;
	pdf = 1.f;
	vec3 pOutside = vp.point + wLight*10000;
	vis.p0.point = vp.point;
	vis.p0.wo = vp.wo;
	vis.p0.n = vp.n;
	vis.p1.point = pOutside;

	return L;
}

vec3 DistantLight_Le(Light light, vec3 rayDirection)
{
	vec3 color = light.datav[0].xyz;
	vec3 dir = light.datav[1].xyz;
	float d = dot(rayDirection, dir);
	d-=0.98;
	if(d<0)d=0;
	d*=50;

	return color + vec3(.2,.2,.2)*d;
}

vec3 DistantLight_power(vec3 L)
{
	return L * 3.1415926535f * 5000*5000;
}

uint DistantLight_flags()
{
	return LIGHT_FLAG_DELTA_POSITION | LIGHT_FLAG_INFINITE;
}

#endif// LIGHTS_DISTANT_GLSL
