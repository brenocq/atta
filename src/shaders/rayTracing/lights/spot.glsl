//--------------------------------------------------
// Atta Ray Tracing Vulkan GLSL
// spot.glsl
// Date: 2021-02-13
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef LIGHTS_SPOT_GLSL
#define LIGHTS_SPOT_GLSL
#include "base.glsl"

float SpotLight_falloff(Light light, vec3 w, float cosTotalWidth, float cosFalloffStart);

vec3 SpotLight_sampleLi(
		Light light, Interaction ref, vec2 u, 
		out vec3 wi, out float pdf, out VisibilityTester vis, 
		vec3 I, float cosTotalWidth, float cosFalloffStart)
{
	vec3 pLight = (light.lightToWorld * vec4(0,0,0,1)).xyz;
	
	wi = normalize(pLight - ref.point);
	pdf = 1.f;
	vis.p0 = ref;
	vis.p1.point = pLight;

	vec3 p = pLight-ref.point;
	return I*SpotLight_falloff(light, -wi, cosTotalWidth, cosFalloffStart)/dot(p,p);
}

vec3 SpotLight_power(vec3 I, float cosTotalWidth, float cosFalloffStart)
{
	return I * 2 * 3.1415926535f * (1 - 0.5f * (cosFalloffStart+cosTotalWidth));
}

uint SpotLight_flags()
{
	return LIGHT_FLAG_DELTA_POSITION;
}

float SpotLight_falloff(Light light, vec3 w, float cosTotalWidth, float cosFalloffStart)
{
	vec3 wl = normalize((light.worldToLight * vec4(w,1)).xyz);
	float cosTheta = wl.z;
	if(cosTheta < cosTotalWidth) return 0;
	if(cosTheta >= cosFalloffStart) return 1;

	// Calculate falloff inside transition area
	float delta = (cosTheta - cosTotalWidth)/(cosFalloffStart - cosTotalWidth);
	return (delta*delta) * (delta*delta);
}

#endif// LIGHTS_SPOT_GLSL
