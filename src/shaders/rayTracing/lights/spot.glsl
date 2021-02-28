//--------------------------------------------------
// Atta Ray Tracing Vulkan GLSL
// spot.glsl
// Date: 2021-02-13
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef LIGHTS_SPOT_GLSL
#define LIGHTS_SPOT_GLSL
#include "base.glsl"

float SpotLight_falloff(Light light, vec3 w, float cosFalloffStart, float cosTotalWidth);

vec3 SpotLight_sampleLi(
		Light light, VisibilityPoint vp, vec2 u, 
		out vec3 wi, out float pdf, out VisibilityTester vis, 
		vec3 I, float cosFalloffStart, float cosTotalWidth)
{
	vec3 pLight = (light.lightToWorld * vec4(0,0,0,1)).xyz;
	
	wi = normalize(pLight - vp.point);
	pdf = 1.f;
	vis.p0.point = vp.point;
	vis.p0.wo = vp.wo;
	vis.p0.n = vp.n;
	vis.p1.point = pLight;
	vis.p1.point = vec3(-1, 3, -1);

	vec3 p = pLight-vp.point;
	float squareDist = dot(p,p);
	return I*SpotLight_falloff(light, -wi, cosFalloffStart, cosTotalWidth)/squareDist;
}

float SpotLight_falloff(Light light, vec3 w, float cosFalloffStart, float cosTotalWidth)
{
	vec3 wl = normalize((light.worldToLight * vec4(w,0)).xyz);
	float cosTheta = wl.z;
	if(cosTheta < cosTotalWidth) return 0;
	if(cosTheta >= cosFalloffStart) return 1;

	// Calculate falloff inside transition area
	float delta = (cosTheta - cosTotalWidth)/(cosFalloffStart - cosTotalWidth);
	return (delta*delta) * (delta*delta);
}

vec3 SpotLight_power(vec3 I, float cosFalloffStart, float cosTotalWidth)
{
	return I * 2 * 3.1415926535f * (1 - 0.5f * (cosFalloffStart+cosTotalWidth));
}

uint SpotLight_flags()
{
	return LIGHT_FLAG_DELTA_POSITION;
}

#endif// LIGHTS_SPOT_GLSL
