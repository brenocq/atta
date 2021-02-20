//--------------------------------------------------
// Atta Ray Tracing Vulkan GLSL
// microfacetReflection.glsl
// Date: 2021-02-17
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef BXDF_MICROFACET_REFLECTION_GLSL
#define BXDF_MICROFACET_REFLECTION_GLSL
#include "../base.glsl"
#include "base.glsl"
#include "fresnel.glsl"
#include "microfacetDistribution.glsl"

vec3 BXDF_MicrofacetReflection_f(vec3 wo, vec3 wi, 
		vec3 R, Fresnel fresnel, MicrofacetDistribution distribution)
{
	float cosThetaO = absCosTheta(wo), cosThetaI = absCosTheta(wi);
	vec3 wh = wi+wo;

	if(cosThetaI == 0 || cosThetaO == 0) return vec3(0,0,0);
	if(wh.x == 0 && wh.y==0 && wh.z==0) return vec3(0,0,0);
	wh = normalize(wh);

	float cosThetaIF = dot(wi, faceForward(wh, vec3(0,0,1)));
	vec3 F = BXDF_Fresnel_evaluate(fresnel, cosThetaIF);
	float D = BXDF_MicrofacetDistribution_D(distribution, wh);
	float G = BXDF_MicrofacetDistribution_G(distribution, wo, wi);
	
	return R * D * G * F / (4*cosThetaI*cosThetaO);
}

vec3 BXDF_MicrofacetReflection_sampleF(vec3 wo, out vec3 wi, vec2 u, out float pdf, 
		vec3 R, Fresnel fresnel, MicrofacetDistribution distribution)
{
	// Sample microfacet orientation wh and reflected direction wi
	if(wo.z == 0) return vec3(0,0,0);
	vec3 wh = BXDF_MicrofacetDistribution_sampleWh(distribution, wo, u);
	if(dot(wo, wh) < 0) return vec3(0,0,0);// Should be rare
	wi = reflect(wo, wh);
	if(!sameHemisphere(wo, wi)) return vec3(0,0,0);

	// Compute PDF of wi for microfacet reflection
	pdf = BXDF_MicrofacetDistribution_pdf(distribution, wo, wh) / (4 * dot(wo, wh));
	return BXDF_MicrofacetReflection_f(wo, wi, R, fresnel, distribution);
}

float BXDF_MicrofacetReflection_pdf(vec3 wo, vec3 wi, MicrofacetDistribution distribution)
{
	if(!sameHemisphere(wo, wi)) return 0;
	vec3 wh = normalize(wo + wi);
	return BXDF_MicrofacetDistribution_pdf(distribution, wo, wh) / (4 * dot(wo, wh));
}

uint BXDF_MicrofacetReflection_flags()
{
	return BXDF_FLAG_REFLECTION | BXDF_FLAG_GLOSSY;
}

#endif// BXDF_MICROFACET_REFLECTION_GLSL
