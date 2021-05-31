//--------------------------------------------------
// Atta Ray Tracing Vulkan GLSL
// lambertianReflection.glsl
// Date: 2021-02-14
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef BXDF_LAMBERTIAN_REFLECTION_GLSL
#define BXDF_LAMBERTIAN_REFLECTION_GLSL
#include "../base.glsl"
#include "base.glsl"

// Lambertian models a perfect diffuse surface that scatters incident illumination equally in all directions
// (not physically plausible)

vec3 BXDF_LambertianReflection_f(vec3 R)
{
	return R * invPi;
}

float BXDF_LambertianReflection_pdf(vec3 wo, vec3 wi)
{
	return sameHemisphere(wo, wi) ? absCosTheta(wi)*invPi : 0;
}

vec3 BXDF_LambertianReflection_sampleF(vec3 wo, out vec3 wi, vec2 u, out float pdf, vec3 R) 
{
	wi = cosineSampleHemisphere(u);
	if(wo.z<0) wi.z*=-1;
	pdf = BXDF_LambertianReflection_pdf(wo, wi);

	return BXDF_LambertianReflection_f(R);
}

vec3 BXDF_LambertianReflection_rho(vec3 R)
{
	return R;
}

uint BXDF_LambertianReflection_flags()
{
	return BXDF_FLAG_REFLECTION | BXDF_FLAG_DIFFUSE;
}

#endif// BXDF_LAMBERTIAN_REFLECTION_GLSL
