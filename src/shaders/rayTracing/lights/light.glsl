//--------------------------------------------------
// Atta Ray Tracing Vulkan GLSL
// light.glsl
// Date: 2021-02-13
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef LIGHTS_LIGHT_GLSL
#define LIGHTS_LIGHT_GLSL
#include "../bxdf/bsdf.glsl"
#include "base.glsl"
#include "point.glsl"
#include "spot.glsl"
#include "distant.glsl"
#include "infinite.glsl"

//---------- Helpers ----------//
bool isSurfaceInteraction()
{
	return !(ray.it.n.x==0 && ray.it.n.y==0 && ray.it.n.z==0);
}

//---------- Light methods ----------//
vec3 Light_sampleLi(Light light, vec2 u, out vec3 wi, out float pdf, out VisibilityTester vis)
{
	// [in]  light - Light info from light buffer
	// [in]  ref - Interaction info at the reference point (surface)
	// [in]  u - [0,1)^2 random numbers from sampler 2D 
	// [out] wi - Incident direction to the light source
	// [out] pdf - Probability density for the light sample that was taken
	// [out] vis - Info to trace shadow rays
	// [out] return - RGB spectrum from sample
	
	VisibilityPoint vp;
	vp.point = ray.it.point;
	vp.wo = ray.it.wo;
	vp.n = ray.it.n;
	
	//return InfiniteLight_sampleLi(light, vp, u, wi, pdf, vis);
	//return vec3(0,0,0);
	switch(light.type)
	{
		case LIGHT_TYPE_POINT:
			{
				vec3 I = light.datav[0].xyz;
				return PointLight_sampleLi(light, vp, u, wi, pdf, vis, I);
			}
		case LIGHT_TYPE_SPOT:
			{
				vec3 I = light.datav[0].xyz;
				float cosFalloffStart = light.dataf[0];
				float cosTotalWidth = light.dataf[1];
				return SpotLight_sampleLi(light, vp, u, wi, pdf, vis, I, cosFalloffStart, cosTotalWidth);
			}
		case LIGHT_TYPE_DISTANT:
			{
				vec3 L = light.datav[0].xyz;
				vec3 wLight = light.datav[1].xyz;
				return DistantLight_sampleLi(light, vp, u, wi, pdf, vis, L, wLight);
			}
		case LIGHT_TYPE_INFINITE:
			return InfiniteLight_sampleLi(light, vp, u, wi, pdf, vis);
		default:
			return vec3(0,0,0);
	}
}

vec3 Light_sampleLe(Light light)
{
	return vec3(0,0,0);
}

vec3 Light_Le(Light light, vec3 rayDirection)
{
	switch(light.type)
	{
		case LIGHT_TYPE_INFINITE:
			return InfiniteLight_Le(light, rayDirection);
		case LIGHT_TYPE_DISTANT:
			return DistantLight_Le(light, rayDirection);
		default:
			return vec3(0,0,0);
	}
}

float Light_pdfLi(Light light, Interaction it, vec3 wi)
{
	switch(light.type)
	{
		case LIGHT_TYPE_INFINITE:
			return InfiniteLight_pdfLi(light, it, wi);
		default:
			return 0.0f;
	}
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
		case LIGHT_TYPE_INFINITE:
				return InfiniteLight_power(light);
		default:
			return vec3(0,0,0);
	}
}

uint Light_flags(Light light)
{
	switch(light.type)
	{
		case LIGHT_TYPE_POINT:
			return PointLight_flags();
		case LIGHT_TYPE_SPOT:
			return SpotLight_flags();
		case LIGHT_TYPE_DISTANT:
			return DistantLight_flags();
		case LIGHT_TYPE_INFINITE:
			return InfiniteLight_flags();
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

vec3 Light_estimateDirect(uint nLights, Light light, vec2 uLight, vec2 uScattering)
{
	uint bsdfFlags = BXDF_FLAG_ALL;
	vec3 Ld = vec3(0,0,0);

	// Sample Light source with multiple importance sampling
	vec3 wi;
	float lightPdf = 0, scatteringPdf = 0;
	VisibilityTester vis;
	vec3 Li = Light_sampleLi(light, uLight, wi, lightPdf, vis);

	if(lightPdf>0 && !isBlack(Li))
	{
		// Compute BSDF or phase function’s value for light sample
		vec3 f;
		if(isSurfaceInteraction())
		{
			// Evaluate BSDF for light sampling strategy
			f = BSDF_f(ray.it.wo, wi, bsdfFlags) * abs(dot(wi, ray.it.n));// TODO use shading normal
			scatteringPdf = BSDF_pdf(ray.it.bsdf, ray.it.wo, wi, bsdfFlags);
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
					float weight = powerHeuristic(1, lightPdf, 1, scatteringPdf);
					Ld += f*Li*weight/lightPdf;
				}
			}
		}
	}

	// TODO sample bsdf with multiple importance sampling
	if(!Light_isDeltaLight(light))
	{
		vec3 f;
		bool sampledSpecular = false;

		if(isSurfaceInteraction())
		{
			// Sample scattered direction for surface interactions
			uint sampledType;
			f = BSDF_sampleF(ray.it.bsdf, ray.it.wo, wi, uScattering, scatteringPdf, bsdfFlags, sampledType);
			f *= abs(dot(wi, ray.it.n));// TODO use shading normal
			sampledSpecular = (sampledType&BXDF_FLAG_SPECULAR)!=0;
		}
		else
		{
			// TODO Sample scattered direction for medium interactions
			
		}

		if(!isBlack(f) && scatteringPdf>0)
		{
			// Account for light contributions along sampled direction wi
			float weight = 1;
			if(!sampledSpecular)
			{
				lightPdf = Light_pdfLi(light, ray.it, wi);
				if(lightPdf == 0) return Ld;
				weight = powerHeuristic(1, scatteringPdf, 1, lightPdf);
			}

			// Find intersection and compute transmittance
			vec3 origin = ray.it.point;
			vec3 direction = wi;
			float tMax = 10000;
			bool foundSurfaceInteraction = Light_occluded(origin, direction, tMax);

			vec3 Li = vec3(0, 0, 0);
			if(foundSurfaceInteraction)
			{
				// TODO Emissive surface
			}
			else
				Li += Light_Le(light, wi);
			if(!isBlack(Li))
				Ld += f*Li*weight/scatteringPdf;
		}
	}

	return Ld;
}

vec3 Light_uniformSampleOneLight(uint nLights, float uLightIndex, vec2 uLight, vec2 uScattering)
{
	uint lightIndex = min(int(uLightIndex * nLights), nLights - 1);
	Light l = lightBuffer[lightIndex];

	float lightPdf = 1.0f/nLights;
	// TODO Get light pdf from light distribution
	return Light_estimateDirect(nLights, l, uLight, uScattering)/lightPdf;
}


#endif// LIGHTS_LIGHT_GLSL
