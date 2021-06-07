//--------------------------------------------------
// Atta Ray Tracing Vulkan GLSL
// glass.glsl
// Date: 2021-06-31
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef BXDF_GLASS_GLSL
#define BXDF_GLASS_GLSL
#include "../base.glsl"
#include "base.glsl"
#include "fresnel.glsl"
#include "fresnelSpecular.glsl"
#include "specularReflection.glsl"
#include "specularTransmission.glsl"
#include "microfacetReflection.glsl"
#include "microfacetTransmission.glsl"

// BXDF_TYPE_GLASS
// - dataf
// 		[0] roughnessu
// 		[1] roughnessv
// 		[2] eta
// 		[3] bumpMap
// - datav
// 		[0] Kr
// 		[1] Kt

vec3 BXDF_Glass_f(vec3 wo, vec3 wi, BXDF bxdf)
{
	vec3 f = vec3(0,0,0);

	vec3 R = bxdf.datav[0];
	vec3 T = bxdf.datav[1];
	float roughu = bxdf.dataf[0];
	float roughv = bxdf.dataf[1];
	float eta = bxdf.dataf[2];

	bool isSpecular = roughu==0 && roughv==0;
	bool allowMultipleLobes = true;
	if(isSpecular && allowMultipleLobes)
	{
		f += BXDF_FresnelSpecular_f();
	}
	else
	{
		Fresnel fresnel;
		fresnel.type = BXDF_FRESNEL_TYPE_DIELETRIC;
		fresnel.data0.x = 1.f;
		fresnel.data1.x = eta;

		MicrofacetDistribution distribution;
		distribution.type = BXDF_MICROFACET_DISTRIBUTION_TYPE_TROWBRIDGE_REITZ;
		distribution.alphaX = roughu;
		distribution.alphaY = roughv;
		distribution.sampleVisibleArea = 1;

		if(!isBlack(R))
		{
			
			if(isSpecular)
			{
				// Sample specular reflection 
				f += BXDF_SpecularReflection_f();
			}
			else
			{
				// Sample microfacet reflection 
				f += BXDF_MicrofacetReflection_f(wo, wi, R, fresnel, distribution);
			}
		}

		if(!isBlack(T))
		{
			if(isSpecular)
			{
				// Sample specular transmission
				f += BXDF_SpecularTransmission_f();
			}
			else
			{
				// Sample microfacet transmission
				f += BXDF_MicrofacetTransmission_f(wo, wi, T, 1.f, eta, distribution);
			}
		}
	}

	return f;
}

vec3 BXDF_Glass_sampleF(vec3 wo, out vec3 wi, vec2 u, out float pdf, BXDF bxdf)
{
	uint seed  = uint(u.x*10000);
	float random = randomFloat(seed);

	vec3 R = bxdf.datav[0];
	vec3 T = bxdf.datav[1];
	float roughu = bxdf.dataf[0];
	float roughv = bxdf.dataf[1];
	float eta = bxdf.dataf[2];
	bool isSpecular = roughu==0 && roughv==0;

	// Sample one BXDF
	int qtyBXDF = int(!isBlack(R)) + int(!isBlack(T));
	int count = min(int(random*qtyBXDF), qtyBXDF-1);

	bool allowMultipleLobes = true;
	if(isSpecular && allowMultipleLobes)
	{
		return BXDF_FresnelSpecular_sampleF(wo, wi, u, pdf, R, T, 1.0f, eta); 
	}
	else
	{
		Fresnel fresnel;
		fresnel.type = BXDF_FRESNEL_TYPE_DIELETRIC;
		fresnel.data0.x = 1.f;
		fresnel.data1.x = eta;

		MicrofacetDistribution distribution;
		distribution.type = BXDF_MICROFACET_DISTRIBUTION_TYPE_TROWBRIDGE_REITZ;
		distribution.alphaX = roughu;
		distribution.alphaY = roughv;
		distribution.sampleVisibleArea = 1;

		if(!isBlack(R) && count--==0)
		{
			if(isSpecular)
			{
				// Sample specular reflection 
				return BXDF_SpecularReflection_sampleF(wo, wi, u, pdf, fresnel, R);
			}
			else
			{
				// Sample microfacet reflection 
				return BXDF_MicrofacetReflection_sampleF(wo, wi, u, pdf, R, fresnel, distribution);
			}
		}

		if(!isBlack(T) && count--==0)
		{
			if(isSpecular)
			{
		  		// Sample specular transmission
		  		return BXDF_SpecularTransmission_sampleF(wo, wi, u, pdf, fresnel, T);
			}
			else
			{
				// Sample microfacet transmission
		  		return BXDF_MicrofacetTransmission_sampleF(wo, wi, u, pdf, T, 1.f, eta, distribution);
			}
		}
	}
}

uint BXDF_Glass_flags()
{
	return BXDF_FLAG_REFLECTION | BXDF_FLAG_TRANSMISSION | BXDF_FLAG_SPECULAR;
}

#endif// BXDF_GLASS_GLSL
