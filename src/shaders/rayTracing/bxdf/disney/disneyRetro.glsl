//--------------------------------------------------
// Atta Ray Tracing Vulkan GLSL
// disneyRetro.glsl
// Date: 2021-02-20
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef BXDF_DISNEY_RETRO_GLSL
#define BXDF_DISNEY_RETRO_GLSL
#include "../base.glsl"

vec3 BXDF_DisneyRetro_f(vec3 wo, vec3 wi, 
		vec3 R, float roughness)
{
	vec3 wh = wi + wo;
	if(wh.x == 0 && wh.y == 0 && wh.z == 0) return vec3(0,0,0);
	wh = normalize(wh);
	float cosThetaD = dot(wi, wh);

	float Fo = schlickWeight(absCosTheta(wo)),
		  Fi = schlickWeight(absCosTheta(wi));
	float Rr = 2 * roughness * cosThetaD * cosThetaD;

	// Burley 2015, eq (4).
	return R * invPi * Rr * (Fo + Fi + Fo * Fi * (Rr - 1));

}

float BXDF_DisneyRetro_pdf(vec3 wo, vec3 wi)
{
	return sameHemisphere(wo, wi) ? absCosTheta(wi) * invPi : 0;
}

vec3 BXDF_DisneyRetro_sampleF(vec3 wo, out vec3 wi, vec2 u, out float pdf, 
		vec3 R, float roughness)
{
	// Cosine-sample the hemisphere, flipping the direction if necessary
	wi = cosineSampleHemisphere(u);
	if (wo.z < 0) wi.z *= -1;
	pdf = BXDF_DisneyRetro_pdf(wo, wi);
	return BXDF_DisneyRetro_f(wo, wi, R, roughness);
}

uint BXDF_DisneyRetro_flags()
{
	return BXDF_FLAG_REFLECTION | BXDF_FLAG_DIFFUSE;
}

#endif// BXDF_DISNEY_RETRO_GLSL
