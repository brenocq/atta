//--------------------------------------------------
// Atta Ray Tracing Vulkan GLSL
// disneyDiffuse.glsl
// Date: 2021-02-20
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef BXDF_DISNEY_DIFFUSE_GLSL
#define BXDF_DISNEY_DIFFUSE_GLSL
#include "../base.glsl"

vec3 BXDF_DisneyDiffuse_f(vec3 wo, vec3 wi, 
		vec3 R)
{
	float Fo = schlickWeight(absCosTheta(wo)),
		  Fi = schlickWeight(absCosTheta(wi));

	// Diffuse fresnel - go from 1 at normal incidence to .5 at grazing.
	// Burley 2015, eq (4).
	return R * invPi * (1 - Fo / 2) * (1 - Fi / 2);
}

float BXDF_DisneyDiffuse_pdf(vec3 wo, vec3 wi)
{
	return sameHemisphere(wo, wi) ? absCosTheta(wi) * invPi : 0;
}

vec3 BXDF_DisneyDiffuse_sampleF(vec3 wo, out vec3 wi, vec2 u, out float pdf, 
		vec3 R)
{
	// Cosine-sample the hemisphere, flipping the direction if necessary
	wi = cosineSampleHemisphere(u);
	if (wo.z < 0) wi.z *= -1;
	pdf = BXDF_DisneyDiffuse_pdf(wo, wi);
	return BXDF_DisneyDiffuse_f(wo, wi, R);
}

uint BXDF_DisneyDiffuse_flags()
{
	return BXDF_FLAG_REFLECTION | BXDF_FLAG_DIFFUSE;
}

#endif// BXDF_DISNEY_DIFFUSE_GLSL
