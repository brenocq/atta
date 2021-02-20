//--------------------------------------------------
// Atta Ray Tracing Vulkan GLSL
// lambertianTransmission.glsl
// Date: 2021-02-20
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef BXDF_LAMBERTIAN_TRANSMISSION_GLSL
#define BXDF_LAMBERTIAN_TRANSMISSION_GLSL
#include "../base.glsl"
#include "base.glsl"

vec3 BXDF_LambertianTransmission_f(vec3 T)
{
	return T * invPi;
}

float BXDF_LambertianTransmission_pdf(vec3 wo, vec3 wi)
{
	return !sameHemisphere(wo, wi) ? absCosTheta(wi) * invPi : 0;  
}

vec3 BXDF_LambertianTransmission_sampleF(vec3 wo, out vec3 wi, vec2 u, out float pdf, vec3 T)
{
	wi = cosineSampleHemisphere(u);
	if (wo.z > 0) wi.z *= -1;
	pdf = BXDF_LambertianTransmission_pdf(wo, wi);
	return BXDF_LambertianTransmission_f(T);
}

uint BXDF_LambertianTransmission_flags()
{
	return BXDF_FLAG_TRANSMISSION | BXDF_FLAG_DIFFUSE;
}

#endif// BXDF_LAMBERTIAN_TRANSMISSION_GLSL
