//--------------------------------------------------
// Atta Ray Tracing Vulkan GLSL
// bxdf.glsl
// Date: 2021-02-13
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef BXDF_BXDF_GLSL
#define BXDF_BXDF_GLSL
#include "base.glsl"
#include "specularReflection.glsl"
#include "specularTransmission.glsl"
#include "lambertianReflection.glsl"
#include "orenNayar.glsl"

uint BXDF_flags(BXDF bxdf);

float BXDF_pdf(BXDF bxdf, vec3 wo, vec3 wi)
{
	switch(bxdf.type)
	{
		default:
			return sameHemisphere(wo, wi) ? absCosTheta(wi)*invPi : 0;
	}
}

vec3 BXDF_f(BXDF bxdf, vec3 wo, vec3 wi)
{
	switch(bxdf.type)
	{
		case BXDF_TYPE_LAMBERTIAN_REFLECTION:
			{
				vec3 R = bxdf.datav[0];
				return BXDF_LambertianReflection_f(R);
			}
		case BXDF_TYPE_OREN_NAYAR:
			{
				vec3 R = bxdf.datav[0];
				float sigma = bxdf.dataf[0];// Standard deviation of the microfacet orientation angle (degrees)
				return BXDF_OrenNayar_f(wo, wi, R, sigma);
			}
		default:
			return vec3(0,0,0);
	}
}

vec3 BXDF_sampleF(BXDF bxdf, vec3 wo, out vec3 wi, vec2 u, out float pdf, out uint sampledType)
{
	switch(bxdf.type)
	{
		case BXDF_TYPE_SPECULAR_REFLECTION:
		{
			Fresnel fresnel;
			fresnel.type = BXDF_FRESNEL_TYPE_DIELETRIC;
			fresnel.etaI = vec3(1,1,1);
			fresnel.etaT = vec3(1.52,1.52,1.52);// Simulate glass

			vec3 R = vec3(2,.5, .8);

			return BXDF_SpecularReflection_sampleF(
				wo, wi, u, pdf, sampledType,
				fresnel, R);
		}
		case BXDF_TYPE_SPECULAR_TRANSMISSION:
		{
			Fresnel fresnel;
			fresnel.type = BXDF_FRESNEL_TYPE_DIELETRIC;
			fresnel.etaI = vec3(1,1,1);
			fresnel.etaT = vec3(1.52,1.52,1.52);// Simulate glass

			vec3 T = vec3(2,.5, .8);

			return BXDF_SpecularReflection_sampleF(
				wo, wi, u, pdf, sampledType,
				fresnel, T);
		}
		case BXDF_TYPE_LAMBERTIAN_REFLECTION:
		case BXDF_TYPE_OREN_NAYAR:
			sampledType = BXDF_flags(bxdf);
			wi = cosineSampleHemisphere(u);
			if(wo.z<0) wi.z*=-1;
			pdf = BXDF_pdf(bxdf, wo, wi);
			return BXDF_f(bxdf, wo, wi);
	}
}

// Hemispherical-directional reflectance
vec3 BXDF_rho(BXDF bxdf, vec3 wo, int nSamples, vec2 samples[8])
{
	switch(bxdf.type)
	{
		case BXDF_TYPE_LAMBERTIAN_REFLECTION:
			{
				vec3 R = vec3(.8, .6, .3);
				return BXDF_LambertianReflection_rho(R);
			}
		default:
			return vec3(0,0,0);
	}
}

// Hemispherical-hemispherical reflectance
vec3 BXDF_rho(BXDF bxdf, int nSamples, vec2 samples0[8], vec2 samples1[8])
{
	switch(bxdf.type)
	{
		case BXDF_TYPE_LAMBERTIAN_REFLECTION:
			{
				vec3 R = vec3(.8, .6, .3);
				return BXDF_LambertianReflection_rho(R);
			}
		default:
			return vec3(0,0,0);
	}
}

uint BXDF_flags(BXDF bxdf)
{
	switch(bxdf.type)
	{
		case BXDF_TYPE_SPECULAR_REFLECTION:
			return BXDF_SpecularReflection_flags();
		case BXDF_TYPE_LAMBERTIAN_REFLECTION:
			return BXDF_LambertianReflection_flags();
		case BXDF_TYPE_OREN_NAYAR:
			return BXDF_OrenNayar_flags();
		default:
			return 0;
	}
}

bool BXDF_matchesFlags(BXDF bxdf, uint flags)
{
	uint bxdfFlags = BXDF_flags(bxdf);
	return (bxdfFlags & flags) == bxdfFlags;
}

#endif// BXDF_BXDF_GLSL
