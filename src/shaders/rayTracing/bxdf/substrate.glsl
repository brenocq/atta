//--------------------------------------------------
// Atta Ray Tracing Vulkan GLSL
// substrate.glsl
// Date: 2021-06-31
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef BXDF_SUBSTRATE_GLSL
#define BXDF_SUBSTRATE_GLSL
#include "../base.glsl"
#include "base.glsl"
#include "fresnelBlend.glsl"

// BXDF_TYPE_SUBSTRATE
// - dataf
// 		[0] roughnessu
// 		[1] roughnessv
// 		[2] bumpMap
// - datav
// 		[0] Kd
// 		[1] Ks

vec3 BXDF_Substrate_f(vec3 wo, vec3 wi, BXDF bxdf)
{
	vec3 d = bxdf.datav[0];
	vec3 s = bxdf.datav[1];
	float roughu = bxdf.dataf[0];
	float roughv = bxdf.dataf[1];

	MicrofacetDistribution distribution;
	distribution.type = BXDF_MICROFACET_DISTRIBUTION_TYPE_TROWBRIDGE_REITZ;
	distribution.alphaX = roughu;
	distribution.alphaY = roughv;
	distribution.sampleVisibleArea = 1;

	return BXDF_FresnelBlend_f(wo, wi, d, s, distribution);
}

float BXDF_Substrate_pdf(vec3 wo, vec3 wi, BXDF bxdf)
{
	float roughu = bxdf.dataf[0];
	float roughv = bxdf.dataf[1];

	MicrofacetDistribution distribution;
	distribution.type = BXDF_MICROFACET_DISTRIBUTION_TYPE_TROWBRIDGE_REITZ;
	distribution.alphaX = roughu;
	distribution.alphaY = roughv;
	distribution.sampleVisibleArea = 1;

	return BXDF_FresnelBlend_pdf(wo, wi, distribution);
}

vec3 BXDF_Substrate_sampleF(vec3 wo, out vec3 wi, vec2 u, out float pdf, BXDF bxdf)
{
	vec3 d = bxdf.datav[0];
	vec3 s = bxdf.datav[1];
	float roughu = bxdf.dataf[0];
	float roughv = bxdf.dataf[1];

	MicrofacetDistribution distribution;
	distribution.type = BXDF_MICROFACET_DISTRIBUTION_TYPE_TROWBRIDGE_REITZ;
	distribution.alphaX = roughu;
	distribution.alphaY = roughv;
	distribution.sampleVisibleArea = 1;

	return BXDF_FresnelBlend_sampleF(wo, wi, u, pdf, d, s, distribution);
}

uint BXDF_Substrate_flags()
{
	return BXDF_FresnelBlend_flags();
}

#endif// BXDF_SUBSTRATE_GLSL
