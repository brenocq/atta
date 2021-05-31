//--------------------------------------------------
// Atta Ray Tracing Vulkan GLSL
// uber.glsl
// Date: 2021-06-30
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef BXDF_UBER_GLSL
#define BXDF_UBER_GLSL
#include "../base.glsl"
#include "base.glsl"
#include "fresnel.glsl"
#include "specularTransmission.glsl"
#include "microfacetReflection.glsl"

vec3 BXDF_Uber_f(vec3 wo, vec3 wi, BXDF bxdf)
{
	vec3 f = vec3(0,0,0);

	vec3 op = bxdf.datav[4];
	vec3 t = 1-op;// 1-opacity
	vec3 kd = op*bxdf.datav[0];
	vec3 ks = op*bxdf.datav[1];
	vec3 kr = op*bxdf.datav[2];
	vec3 kt = op*bxdf.datav[3];

	// Sample specular transmission opacity
	if(!isBlack(t))
	{
		f += BXDF_SpecularTransmission_f();
	}
	// Sample lambiertian reflection
	if(!isBlack(kd))
	{
		f += BXDF_LambertianReflection_f(kd);
	}
	// Sample microfacet reflection
	if(!isBlack(ks))
	{
		float roughu = bxdf.dataf[0];
		float roughv = bxdf.dataf[1];
		float eta = bxdf.dataf[2];

		Fresnel fresnel;
		fresnel.type = BXDF_FRESNEL_TYPE_DIELETRIC;
		fresnel.data0.x = 1.f;
		fresnel.data1.x = eta;

		MicrofacetDistribution distribution;
		distribution.type = BXDF_MICROFACET_DISTRIBUTION_TYPE_TROWBRIDGE_REITZ;
		distribution.alphaX = roughu;
		distribution.alphaY = roughv;
		distribution.sampleVisibleArea = 1;

		f += BXDF_MicrofacetReflection_f(wo, wi, ks, fresnel, distribution);
	}
	// Sample specular reflection
	if(!isBlack(kr))
	{
		f += BXDF_SpecularReflection_f();
	}
	// Sample specular transmission
	if(!isBlack(kt))
	{
		f += BXDF_SpecularTransmission_f();
	}

	return f;
}

vec3 BXDF_Uber_sampleF(vec3 wo, out vec3 wi, vec2 u, out float pdf, BXDF bxdf)
{
	uint seed  = uint(u.x*10000);
	float random = randomFloat(seed);

	vec3 op = bxdf.datav[4];
	vec3 t = 1-op;// 1-opacity
	vec3 kd = op*bxdf.datav[0];
	vec3 ks = op*bxdf.datav[1];
	vec3 kr = op*bxdf.datav[2];
	vec3 kt = op*bxdf.datav[3];

	// Sample one BXDF
	int qtyBXDF = int(!isBlack(t)) + int(!isBlack(kd))+ int(!isBlack(ks)) + int(!isBlack(kr)) + int(!isBlack(kt));
	int count = min(int(random*qtyBXDF), qtyBXDF-1);

	// Sample specular transmission opacity
	if(!isBlack(t) && count--==0)	
	{
		Fresnel fresnel;
		fresnel.type = BXDF_FRESNEL_TYPE_DIELETRIC;
		fresnel.data0.x = 1.0f;
		fresnel.data1.x = 1.0f;
		return BXDF_SpecularTransmission_sampleF(wo, wi, u, pdf, fresnel, t);
	}
	// Sample lambiertian reflection
	if(!isBlack(kd) && count--==0)	
	{
		return BXDF_LambertianReflection_sampleF(wo, wi, u, pdf, kd);
	}
	// Sample microfacet reflection
	if(!isBlack(ks) && count--==0)
	{
		float roughu = bxdf.dataf[0];
		float roughv = bxdf.dataf[1];
		float eta = bxdf.dataf[2];

		Fresnel fresnel;
		fresnel.type = BXDF_FRESNEL_TYPE_DIELETRIC;
		fresnel.data0.x = 1.f;
		fresnel.data1.x = eta;

		MicrofacetDistribution distribution;
		distribution.type = BXDF_MICROFACET_DISTRIBUTION_TYPE_TROWBRIDGE_REITZ;
		distribution.alphaX = roughu;
		distribution.alphaY = roughv;
		distribution.sampleVisibleArea = 1;

		return BXDF_MicrofacetReflection_sampleF(wo, wi, u, pdf, ks, fresnel, distribution);
	}
	// Sample specular reflection
	if(!isBlack(kr) && count--==0)	
	{
		float eta = bxdf.dataf[2];
		Fresnel fresnel;
		fresnel.type = BXDF_FRESNEL_TYPE_DIELETRIC;
		fresnel.data0.x = 1.f;
		fresnel.data1.x = eta;
		
		return BXDF_SpecularReflection_sampleF(wo, wi, u, pdf, fresnel, kr);
	}
	// Sample specular transmission
	if(!isBlack(kt) && count--==0)	
	{
		float eta = bxdf.dataf[2];
		Fresnel fresnel;
		fresnel.type = BXDF_FRESNEL_TYPE_DIELETRIC;
		fresnel.data0.x = 1.0f;
		fresnel.data1.x = eta;
		return BXDF_SpecularTransmission_sampleF(wo, wi, u, pdf, fresnel, t);
	}
}

uint BXDF_Uber_flags()
{
	return BXDF_FLAG_REFLECTION | BXDF_FLAG_TRANSMISSION | BXDF_FLAG_SPECULAR | BXDF_FLAG_DIFFUSE;
}

#endif// BXDF_UBER_GLSL
