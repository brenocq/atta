//--------------------------------------------------
// Atta Ray Tracing Vulkan GLSL
// orenNayar.glsl
// Date: 2021-02-14
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef BXDF_OREN_NAYAR_GLSL
#define BXDF_OREN_NAYAR_GLSL
#include "../base.glsl"
#include "base.glsl"

void BXDF_OrenNayar_precompute(vec3 R, float sigma, out float A, out float B)
{
	// TODO Precompute A and B only one time
	sigma = radians(sigma);
	float sigma2 = sigma*sigma;
	A = 1.f - (sigma2/(2.f*(sigma2+0.33f)));
	B = 0.45 * sigma2/(sigma2 + 0.09);
}

vec3 BXDF_OrenNayar_f(vec3 wo, vec3 wi, vec3 R, float sigma)
{
	float A, B;
	BXDF_OrenNayar_precompute(R, sigma, A, B);
	float sinThetaI = sinTheta(wi);
	float sinThetaO = sinTheta(wo);

	// Compute cosine term of Oren–Nayar model
	float maxCos = 0;
	if(sinThetaI > 1e-4 && sinThetaO > 1e-4) {
		float sinPhiI = sinPhi(wi), cosPhiI = cosPhi(wi);
		float sinPhiO = sinPhi(wo), cosPhiO = cosPhi(wo);
		float dCos = cosPhiI * cosPhiO + sinPhiI * sinPhiO;
		maxCos = max(0.f, dCos);
	}
	
	// Compute sine and tangent terms of Oren–Nayar model
	float sinAlpha, tanBeta;
	if(absCosTheta(wi) > absCosTheta(wo))
	{
		sinAlpha = sinThetaO;
		tanBeta = sinThetaI / absCosTheta(wi);
	}
	else
	{
		sinAlpha = sinThetaI;
		tanBeta = sinThetaO / absCosTheta(wo);
	}
	return R * invPi * (A + B * maxCos * sinAlpha * tanBeta); 
}

float BXDF_OrenNayar_pdf(vec3 wo, vec3 wi)
{
	return sameHemisphere(wo, wi) ? absCosTheta(wi)*invPi : 0;
}

vec3 BXDF_OrenNayar_sampleF(vec3 wo, out vec3 wi, vec2 u, out float pdf, vec3 R, float sigma) 
{
	wi = cosineSampleHemisphere(u);
	if(wo.z<0) wi.z*=-1;
	pdf = BXDF_OrenNayar_pdf(wo, wi);

	return BXDF_OrenNayar_f(wo, wi, R, sigma);
}

uint BXDF_OrenNayar_flags()
{
	return BXDF_FLAG_REFLECTION | BXDF_FLAG_DIFFUSE;
}

#endif// BXDF_OREN_NAYAR_GLSL
