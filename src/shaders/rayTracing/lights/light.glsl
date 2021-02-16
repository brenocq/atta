//--------------------------------------------------
// Atta Ray Tracing Vulkan GLSL
// light.glsl
// Date: 2021-02-13
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef LIGHTS_LIGHT_GLSL
#define LIGHTS_LIGHT_GLSL
#include "../base.glsl"
#include "../bxdf/bsdf.glsl"
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
				vec3 I = light.datav[0].xyz;
				return PointLight_sampleLi(light, ref, u, wi, pdf, vis, I);
			}
		case LIGHT_TYPE_SPOT:
			{
				vec3 I = light.datav[0].xyz;
				float cosFalloffStart = light.dataf[0];
				float cosTotalWidth = light.dataf[1];
				return SpotLight_sampleLi(light, ref, u, wi, pdf, vis, I, cosFalloffStart, cosTotalWidth);
			}
		case LIGHT_TYPE_DISTANT:
			{
				vec3 L = light.datav[0].xyz;
				vec3 wLight = light.datav[1].xyz;
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

float Light_pdfLi(Light light)
{

	return 0.f;
}

void Light_pdfLe(Light light)
{

}

vec3 Light_power(Light light)
{
	switch(light.type)
	{
		case LIGHT_TYPE_POINT:
			{
				vec3 I = light.datav[0].xyz;
				return PointLight_power(I);
			}
		case LIGHT_TYPE_SPOT:
			{
				vec3 I = light.datav[0].xyz;
				float cosFalloffStart = light.dataf[0];
				float cosTotalWidth = light.dataf[1];
				return SpotLight_power(I, cosFalloffStart, cosTotalWidth);
			}
		case LIGHT_TYPE_DISTANT:
			{
				vec3 L = light.datav[0].xyz;
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

bool Light_isDeltaLight(Light light)
{
	uint lFlags = Light_flags(light);
	return (lFlags & LIGHT_FLAG_DELTA_POSITION)!=0 ||
			(lFlags & LIGHT_FLAG_DELTA_DIRECTION)!=0;
}

//---------- Sampling lights ----------//
bool Light_occluded(vec3 origin, vec3 direction, float tMax)
{
	float tMin = 0.001;

	isShadowed = true;	
	uint flags = gl_RayFlagsTerminateOnFirstHitEXT | gl_RayFlagsOpaqueEXT | gl_RayFlagsSkipClosestHitShaderEXT;
	traceRayEXT(scene,   // acceleration structure
            flags,       // rayFlags
            0xFF,        // cullMask
            0,           // sbtRecordOffset
            0,           // sbtRecordStride
            1,           // missIndex
            origin,      // ray origin
            tMin,        // ray min range
            direction,   // ray direction
            tMax,        // ray max range
            1            // payload (location = 1)
    );

	return isShadowed;
}

vec3 Light_estimateDirect(uint nLights, Light light, Interaction it, vec2 uLight, vec2 uScattering)
{
	uint bsdfFlags = BXDF_FLAG_ALL;
	vec3 Ld = vec3(0,0,0);

	// Sample Light source with multiple importance sampling
	vec3 wi;
	float lightPdf = 0, scatteringPdf = 0;
	VisibilityTester vis;
	vec3 Li = Light_sampleLi(light, it, uLight, wi, lightPdf, vis);

	if(lightPdf>0 && !isBlack(Li))
	{
		// Compute BSDF or phase function’s value for light sample
		vec3 f;
		if(isSurfaceInteraction(it))
		{
			// Evaluate BSDF for light sampling strategy
			f = BSDF_f(it.bsdf, it.wo, wi, bsdfFlags);
			//scatteringPdf = BSDF_pdf(it.bsdf, it.wo, wi, bsdfFlags);// TODO implement (used only for area lights)
		}
		else
		{
			// TODO Evaluate phase function for light sampling strategy (Medium)
			
		}

		if(!isBlack(f))
		{
			// Compute effect of visibility for light source sample
			vec3 origin = vis.p0.point;
			vec3 direction = normalize(vis.p1.point - vis.p0.point);
			float tMax = length(vis.p1.point - vis.p0.point);
			if(Light_occluded(origin, direction, tMax))
				Li = vec3(0,0,0);

			// Add light contribution to reflected radiance
			if(!isBlack(Li))
			{
				if(Light_isDeltaLight(light))
				{
					Ld += f*Li/lightPdf;
				}
				else
				{
					// TODO area lights
					//float weight = powerHeuristic(1, lightPdf, 1, scatteringPdf);
					//Ld += f*Li*wight/lightPdf;
				}
			}
		}
	}

	// TODO sample bsdf with multiple importance sampling
	if(!Light_isDeltaLight(light))
	{

	}

	return Ld;
}

vec3 Light_uniformSampleOneLight(uint nLights, Interaction it, float uLightIndex, vec2 uLight, vec2 uScattering)
{
	uint lightIndex = min(int(uLightIndex * nLights), nLights - 1);
	Light l = lightBuffer[lightIndex];

	return nLights * Light_estimateDirect(nLights, l, it, uLight, uScattering);
}


#endif// LIGHTS_LIGHT_GLSL
