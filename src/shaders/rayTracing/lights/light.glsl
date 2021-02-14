//--------------------------------------------------
// Atta Ray Tracing Vulkan GLSL
// light.glsl
// Date: 2021-02-13
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef LIGHTS_LIGHT_GLSL
#define LIGHTS_LIGHT_GLSL
#include "base.glsl"
#include "point.glsl"
#include "spot.glsl"
#include "distant.glsl"

vec3 Light_sampleLi(Light light, Interaction ref, vec2 u, out vec3 wi, out float pdf, out VisibilityTester vis)
{
	// [in]  light - Light info from light buffer
	// [in]  ref - Interaction info at the reference point (surface)
	// [in]  u - [0,1)^2 random numbers from sampler 2D 
	// [out] wi - Incident direction to the light source
	// [out] pdf - Probability density for the light sample that was taken
	// [out] vis - Info to trace shadow rays
	// [out] return - RGB spectrum from sample
	
	switch(light.type)
	{
		case LIGHT_TYPE_POINT:
			{
				// I - Light intensity (amount of power per unit solid angle, constant for isotropic point lights)
				vec3 I = vec3(1,1,1);
				return PointLight_sampleLi(light, ref, u, wi, pdf, vis, I);
			}
		case LIGHT_TYPE_SPOT:
			{
				vec3 I = vec3(1,1,1);
				float cosFalloffStart = 0.5;
				float cosTotalWidth = 0.8;
				return SpotLight_sampleLi(light, ref, u, wi, pdf, vis, I, cosFalloffStart, cosTotalWidth);
			}
		case LIGHT_TYPE_DISTANT:
			{
				vec3 L = vec3(1,1,1);
				vec3 wLight = normalize(vec3(1, -2, 1));
				return DistantLight_sampleLi(light, ref, u, wi, pdf, vis, L, wLight);
			}
		default:
			return vec3(0,0,0);
	}
}

vec3 Light_sampleLe(Light light)
{
	return vec3(0,0,0);
}

vec3 Light_Le(Light light)
{

	return vec3(0,0,0);
}

float Light_PdfLi(Light light)
{

	return 0.f;
}

void Light_PdfLe(Light light)
{

}

vec3 Light_power(Light light)
{
	switch(light.type)
	{
		case LIGHT_TYPE_POINT:
			{
				vec3 I = vec3(1,1,1);
				return PointLight_power(I);
			}
		case LIGHT_TYPE_SPOT:
			{
				vec3 I = vec3(1,1,1);
				float cosFalloffStart = 0.5;
				float cosTotalWidth = 0.8;
				return SpotLight_power(I, cosFalloffStart, cosTotalWidth);
			}
		case LIGHT_TYPE_DISTANT:
			{
				vec3 L = vec3(1,1,1);
				return DistantLight_power(L);
			}
		default:
			return vec3(0,0,0);
	}
}

uint Light_flags(Light light)
{
	return LIGHT_FLAG_DELTA_POSITION;
	switch(light.type)
	{
		case LIGHT_TYPE_POINT:
			return PointLight_flags();
		case LIGHT_TYPE_SPOT:
			return SpotLight_flags();
		case LIGHT_TYPE_DISTANT:
			return DistantLight_flags();
		default:
			return 0;
	}
}

#endif// LIGHTS_LIGHT_GLSL
