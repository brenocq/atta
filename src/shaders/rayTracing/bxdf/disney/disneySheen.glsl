//--------------------------------------------------
// Atta Ray Tracing Vulkan GLSL
// disneySheen.glsl
// Date: 2021-02-20
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef BXDF_DISNEY_SHEEN_GLSL
#define BXDF_DISNEY_SHEEN_GLSL
#include "../base.glsl"

vec3 BXDF_DisneySheen_f(vec3 wo, vec3 wi, 
		vec3 R)
{
	vec3 wh = wi + wo;
	if(wh.x == 0 && wh.y == 0 && wh.z == 0) return vec3(0,0,0);
	wh = normalize(wh);
	float cosThetaD = dot(wi, wh);

	return R * schlickWeight(cosThetaD);
}

float BXDF_DisneySheen_pdf(vec3 wo, vec3 wi)
{
	return sameHemisphere(wo, wi) ? absCosTheta(wi) * invPi : 0;
}

vec3 BXDF_DisneySheen_sampleF(vec3 wo, out vec3 wi, vec2 u, out float pdf, 
		vec3 R)
{
	// Cosine-sample the hemisphere, flipping the direction if necessary
	wi = cosineSampleHemisphere(u);
	if (wo.z < 0) wi.z *= -1;
	pdf = BXDF_DisneySheen_pdf(wo, wi);
	return BXDF_DisneySheen_f(wo, wi, R);
}

uint BXDF_DisneySheen_flags()
{
	return BXDF_FLAG_REFLECTION | BXDF_FLAG_DIFFUSE;
}

#endif// BXDF_DISNEY_SHEEN_GLSL
