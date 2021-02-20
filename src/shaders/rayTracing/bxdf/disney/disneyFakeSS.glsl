//--------------------------------------------------
// Atta Ray Tracing Vulkan GLSL
// disneyFakeSS.glsl
// Date: 2021-02-20
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef BXDF_DISNEY_FAKESS_GLSL
#define BXDF_DISNEY_FAKESS_GLSL
#include "../base.glsl"

vec3 BXDF_DisneyFakeSS_f(vec3 wo, vec3 wi, 
		vec3 R, float roughness)
{
	vec3 wh = wi + wo;
	if(wh.x == 0 && wh.y == 0 && wh.z == 0) return vec3(0,0,0);
	wh = normalize(wh);
	float cosThetaD = dot(wi, wh);

	// Fss90 used to "flatten" retroreflection based on roughness
	float Fss90 = cosThetaD * cosThetaD * roughness;
	float Fo = schlickWeight(absCosTheta(wo)),
		  Fi = schlickWeight(absCosTheta(wi));
	float Fss = mix(1.0, Fss90, Fo) * mix(1.0, Fss90, Fi);
	// 1.25 scale is used to (roughly) preserve albedo
	float ss =
		1.25f * (Fss * (1 / (absCosTheta(wo) + absCosTheta(wi)) - 0.5f) + 0.5f);

	return R * invPi * ss;
}

float BXDF_DisneyFakeSS_pdf(vec3 wo, vec3 wi)
{
	return sameHemisphere(wo, wi) ? absCosTheta(wi) * invPi : 0;
}

vec3 BXDF_DisneyFakeSS_sampleF(vec3 wo, out vec3 wi, vec2 u, out float pdf, 
		vec3 R, float roughness)
{
	// Cosine-sample the hemisphere, flipping the direction if necessary
	wi = cosineSampleHemisphere(u);
	if (wo.z < 0) wi.z *= -1;
	pdf = BXDF_DisneyFakeSS_pdf(wo, wi);
	return BXDF_DisneyFakeSS_f(wo, wi, R, roughness);
}

uint BXDF_DisneyFakeSS_flags()
{
	return BXDF_FLAG_REFLECTION | BXDF_FLAG_DIFFUSE;
}

#endif// BXDF_DISNEY_FAKESS_GLSL
