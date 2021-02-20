//--------------------------------------------------
// Atta Ray Tracing Vulkan GLSL
// microfacetTransmission.glsl
// Date: 2021-02-19
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef BXDF_MICROFACET_TRANSMISSION_GLSL
#define BXDF_MICROFACET_TRANSMISSION_GLSL
#include "../base.glsl"
#include "base.glsl"
#include "fresnel.glsl"
#include "microfacetDistribution.glsl"

vec3 BXDF_MicrofacetTransmission_f(vec3 wo, vec3 wi, 
		vec3 T, float etaA, float etaB, MicrofacetDistribution distribution)
{
	if(sameHemisphere(wo, wi)) return vec3(0,0,0);  // transmission only 

	float cosThetaO = cosTheta(wo); 
	float cosThetaI = cosTheta(wi); 
	if(cosThetaI == 0 || cosThetaO == 0) return vec3(0,0,0);

	// Compute wh from wo and wi for microfacet transmission
	float eta = cosTheta(wo) > 0 ? (etaB / etaA) : (etaA / etaB);
	vec3 wh = normalize(wo + wi * eta);
	if(wh.z < 0) wh = -wh;

	// Same side?          
	if(dot(wo, wh) * dot(wi, wh) > 0) return vec3(0,0,0);

	Fresnel fresnel;
	fresnel.type = BXDF_FRESNEL_TYPE_DIELETRIC;
	fresnel.data0.x = etaA;
	fresnel.data1.x = etaB;

	vec3 F = BXDF_Fresnel_evaluate(fresnel, dot(wo, wh));
	float D = BXDF_MicrofacetDistribution_D(distribution, wh);
	float G = BXDF_MicrofacetDistribution_G(distribution, wo, wi);

	float sqrtDenom = dot(wo, wh) + eta * dot(wi, wh);
	//float factor = (mode == TransportMode::Radiance) ? (1 / eta) : 1;
	// TODO transport mode
	float factor = (1 / eta);

	return (vec3(1,1,1) - F) * T * abs(D * G * eta * eta *
					abs(dot(wi, wh)) * abs(dot(wo, wh)) * factor * factor /
					(cosThetaI * cosThetaO * sqrtDenom * sqrtDenom));

}

float BXDF_MicrofacetTransmission_pdf(vec3 wo, vec3 wi, float etaA, float etaB, MicrofacetDistribution distribution)
{
	if(sameHemisphere(wo, wi)) return 0;
	// Compute wh from wo and wi for microfacet transmission
	float eta = cosTheta(wo) > 0 ? (etaB / etaA) : (etaA / etaB);
	vec3 wh = normalize(wo + wi * eta);

	if(dot(wo, wh) * dot(wi, wh) > 0) return 0;

	// Compute change of variables dwh\dwi for microfacet transmission
	float sqrtDenom = dot(wo, wh) + eta * dot(wi, wh);
	float dwh_dwi = abs((eta * eta * dot(wi, wh)) / (sqrtDenom * sqrtDenom));
	return BXDF_MicrofacetDistribution_pdf(distribution, wo, wh) * dwh_dwi;
}


vec3 BXDF_MicrofacetTransmission_sampleF(vec3 wo, out vec3 wi, vec2 u, out float pdf, 
		vec3 T, float etaA, float etaB, MicrofacetDistribution distribution)
{
	if (wo.z == 0) return vec3(0,0,0);
	vec3 wh = BXDF_MicrofacetDistribution_sampleWh(distribution, wo, u);
	if(dot(wo, wh) < 0) return vec3(0,0,0);  // Should be rare

	float eta = cosTheta(wo) > 0 ? (etaA / etaB) : (etaB / etaA);
	if (!refractWi(wo, wh, eta, wi)) return vec3(0,0,0);
	pdf = BXDF_MicrofacetTransmission_pdf(wo, wi, etaA, etaB, distribution);
	return BXDF_MicrofacetTransmission_f(wo, wi, T, etaA, etaB, distribution);
}

uint BXDF_MicrofacetTransmission_flags()
{
	return BXDF_FLAG_TRANSMISSION | BXDF_FLAG_GLOSSY;
}

#endif// BXDF_MICROFACET_TRANSMISSION_GLSL
