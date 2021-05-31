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
#include "lambertianTransmission.glsl"
#include "orenNayar.glsl"
#include "microfacetReflection.glsl"
#include "microfacetTransmission.glsl"
#include "disney/disney.glsl"
#include "uber.glsl"
#include "glass.glsl"

uint BXDF_flags(BXDF bxdf);

float BXDF_pdf(BXDF bxdf, vec3 wo, vec3 wi)
{
	switch(bxdf.type)
	{
		case BXDF_TYPE_MICROFACET_REFLECTION:
			{
				MicrofacetDistribution distribution;
				distribution.type = bxdf.datai[1];
				distribution.alphaX = bxdf.dataf[0];
				distribution.alphaY = bxdf.dataf[1];
				distribution.sampleVisibleArea = bxdf.datai[2];
				return BXDF_MicrofacetReflection_pdf(wo, wi, distribution);
			}
		case BXDF_TYPE_MICROFACET_TRANSMISSION:
			{
				MicrofacetDistribution distribution;
				distribution.type = bxdf.datai[1];
				distribution.alphaX = bxdf.dataf[0];
				distribution.alphaY = bxdf.dataf[1];
				distribution.sampleVisibleArea = bxdf.datai[2];

				float etaA = bxdf.dataf[2];
				float etaB = bxdf.dataf[3];

				return BXDF_MicrofacetTransmission_pdf(wo, wi, etaA, etaB, distribution);
			}
		case BXDF_TYPE_LAMBERTIAN_TRANSMISSION:
			return BXDF_LambertianTransmission_pdf(wo, wi);
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
		case BXDF_TYPE_LAMBERTIAN_TRANSMISSION:
			{
				vec3 T = bxdf.datav[0];
				return BXDF_LambertianTransmission_f(T);
			}
		case BXDF_TYPE_OREN_NAYAR:
			{
				vec3 R = bxdf.datav[0];
				float sigma = bxdf.dataf[0];// Standard deviation of the microfacet orientation angle (degrees)
				return BXDF_OrenNayar_f(wo, wi, R, sigma);
			}
		case BXDF_TYPE_MICROFACET_REFLECTION:
			{
				vec3 R = bxdf.datav[0];

				Fresnel fresnel;
				fresnel.type = bxdf.datai[0];
				fresnel.data0 = bxdf.datav[1];
				fresnel.data1 = bxdf.datav[2];
				fresnel.data2 = bxdf.datav[3];

				MicrofacetDistribution distribution;
				distribution.type = bxdf.datai[1];
				distribution.alphaX = bxdf.dataf[0];
				distribution.alphaY = bxdf.dataf[1];
				distribution.sampleVisibleArea = bxdf.datai[2];
				return BXDF_MicrofacetReflection_f(wo, wi, R, fresnel, distribution);
			}
		case BXDF_TYPE_MICROFACET_TRANSMISSION:
			{
				vec3 T = bxdf.datav[0];

				MicrofacetDistribution distribution;
				distribution.type = bxdf.datai[1];
				distribution.alphaX = bxdf.dataf[0];
				distribution.alphaY = bxdf.dataf[1];
				distribution.sampleVisibleArea = bxdf.datai[2];

				float etaA = bxdf.dataf[2];
				float etaB = bxdf.dataf[3];

				return BXDF_MicrofacetTransmission_f(wo, wi, T, etaA, etaB, distribution);
			}
		case BXDF_TYPE_DISNEY:
			return BXDF_Disney_f(wo, wi, bxdf);
		case BXDF_TYPE_UBER:
			return BXDF_Uber_f(wo, wi, bxdf);
		case BXDF_TYPE_GLASS:
			return BXDF_Glass_f(wo, wi, bxdf);
		default:
			return vec3(0,0,0);
	}
}

vec3 BXDF_sampleF(BXDF bxdf, vec3 wo, out vec3 wi, vec2 u, out float pdf, out uint sampledType)
{
	sampledType = BXDF_flags(bxdf);
	switch(bxdf.type)
	{
		case BXDF_TYPE_SPECULAR_REFLECTION:
		{
			Fresnel fresnel;
			fresnel.type = bxdf.datai[0];
			fresnel.data0 = bxdf.datav[1];
			fresnel.data1 = bxdf.datav[2];
			fresnel.data2 = bxdf.datav[3];

			vec3 R = bxdf.datav[0];

			return BXDF_SpecularReflection_sampleF(
				wo, wi, u, pdf,
				fresnel, R);
		}
		case BXDF_TYPE_SPECULAR_TRANSMISSION:
		{
			vec3 T = bxdf.datav[0];

			MicrofacetDistribution distribution;
			distribution.type = bxdf.datai[1];
			distribution.alphaX = bxdf.dataf[0];
			distribution.alphaY = bxdf.dataf[1];
			distribution.sampleVisibleArea = bxdf.datai[2];

			float etaA = bxdf.dataf[2];
			float etaB = bxdf.dataf[3];

			return BXDF_MicrofacetTransmission_sampleF(
				wo, wi, u, pdf,
				T, etaA, etaB, distribution);
		}
		case BXDF_TYPE_MICROFACET_REFLECTION:
			{
				vec3 R = bxdf.datav[0];

				Fresnel fresnel;
				fresnel.type = bxdf.datai[0];
				fresnel.data0 = bxdf.datav[1];
				fresnel.data1 = bxdf.datav[2];
				fresnel.data2 = bxdf.datav[3];

				MicrofacetDistribution distribution;
				distribution.type = bxdf.datai[1];
				distribution.alphaX = bxdf.dataf[0];
				distribution.alphaY = bxdf.dataf[1];
				distribution.sampleVisibleArea = bxdf.datai[2];
				return BXDF_MicrofacetReflection_sampleF(wo, wi, u, pdf, R, fresnel, distribution);
			}
		case BXDF_TYPE_DISNEY:
			return BXDF_Disney_sampleF(wo, wi, u, pdf, bxdf);
		case BXDF_TYPE_UBER:
			return BXDF_Uber_sampleF(wo, wi, u, pdf, bxdf);
		case BXDF_TYPE_GLASS:
			return BXDF_Glass_sampleF(wo, wi, u, pdf, bxdf);
		case BXDF_TYPE_LAMBERTIAN_TRANSMISSION:
			{
				vec3 T = bxdf.datav[0];
				return BXDF_LambertianTransmission_sampleF(wo, wi, u, pdf, T);
			}
		case BXDF_TYPE_LAMBERTIAN_REFLECTION:
			{
				vec3 R = bxdf.datav[0];
				return BXDF_LambertianReflection_sampleF(wo, wi, u, pdf, R);
			}
		case BXDF_TYPE_OREN_NAYAR:
		default:
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
		case BXDF_TYPE_SPECULAR_TRANSMISSION:
			return BXDF_SpecularTransmission_flags();
		case BXDF_TYPE_LAMBERTIAN_REFLECTION:
			return BXDF_LambertianReflection_flags();
		case BXDF_TYPE_OREN_NAYAR:
			return BXDF_OrenNayar_flags();
		case BXDF_TYPE_MICROFACET_REFLECTION:
			return BXDF_MicrofacetReflection_flags();
		case BXDF_TYPE_MICROFACET_TRANSMISSION:
			return BXDF_MicrofacetTransmission_flags();
		case BXDF_TYPE_LAMBERTIAN_TRANSMISSION:
			return BXDF_LambertianTransmission_flags();
		case BXDF_TYPE_DISNEY:
			return BXDF_Disney_flags();
		case BXDF_TYPE_UBER:
			return BXDF_Uber_flags();
		case BXDF_TYPE_GLASS:
			return BXDF_Uber_flags();
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
