//--------------------------------------------------
// Atta Ray Tracing Vulkan GLSL
// disneyClearcoat.glsl
// Date: 2021-02-19
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef BXDF_DISNEY_CLEARCOAT_GLSL
#define BXDF_DISNEY_CLEARCOAT_GLSL
#include "../base.glsl"
#include "base.glsl"

float GTR1(float cosTheta, float alpha)
{
	float alpha2 = alpha * alpha;
    return (alpha2 - 1) / (pi * log(alpha2) * (1 + (alpha2 - 1) * cosTheta * cosTheta));
}

// Smith masking/shadowing term
float smithG_GGX(float cosTheta, float alpha)
{
    float alpha2 = alpha * alpha;
    float cosTheta2 = cosTheta * cosTheta;
    return 1 / (cosTheta + sqrt(alpha2 + cosTheta2 - alpha2 * cosTheta2));
}

vec3 BXDF_DisneyClearcoat_f(vec3 wo, vec3 wi, 
		float weight, float gloss)
{
	vec3 wh = wi + wo;
	if(wh.x == 0 && wh.y == 0 && wh.z == 0) return vec3(0,0,0);
	wh = normalize(wh);

	// Clearcoat has ior = 1.5 hardcoded -> F0 = 0.04. It then uses the
	// GTR1 distribution, which has even fatter tails than Trowbridge-Reitz
	// (which is GTR2).
	float Dr = GTR1(absCosTheta(wh), gloss);
	float Fr = FrSchlick(.04, dot(wo, wh));
	// The geometric term always based on alpha = 0.25.
	float Gr = smithG_GGX(absCosTheta(wo), .25) * smithG_GGX(absCosTheta(wi), .25);

	float f = weight * Gr * Fr * Dr / 4;
	return vec3(f,f,f);
}

float BXDF_DisneyClearcoat_pdf(vec3 wo, vec3 wi, float gloss)
{
    if(!sameHemisphere(wo, wi)) return 0;

    vec3 wh = wi + wo;
    if(wh.x == 0 && wh.y == 0 && wh.z == 0) return 0;
    wh = normalize(wh);

    // The sampling routine samples wh exactly from the GTR1 distribution.
    // Thus, the final value of the PDF is just the value of the
    // distribution for wh converted to a mesure with respect to the
    // surface normal.
    float Dr = GTR1(absCosTheta(wh), gloss);
    return Dr * absCosTheta(wh) / (4 * dot(wo, wh));
}


vec3 BXDF_DisneyClearcoat_sampleF(vec3 wo, out vec3 wi, vec2 u, out float pdf, 
		float weight, float gloss)
{
	// TODO: double check all this: there still seem to be some very
	// occasional fireflies with clearcoat; presumably there is a bug
	// somewhere.
	if (wo.z == 0) return vec3(0,0,0);

	float alpha2 = gloss * gloss;
	float cosTheta = sqrt(max(0.0f, (1 - pow(alpha2, 1 - u.x)) / (1 - alpha2)));
	float sinTheta = sqrt(max(0.0f, 1 - cosTheta * cosTheta));
	float phi = 2 * pi * u.y;
	vec3 wh = sphericalDirection(sinTheta, cosTheta, phi);
	if (!sameHemisphere(wo, wh)) wh = -wh;

	wi = reflect(wo, wh);
	if (!sameHemisphere(wo, wi)) return vec3(0,0,0);

	pdf = BXDF_DisneyClearcoat_pdf(wo, wi, gloss);
	return BXDF_DisneyClearcoat_f(wo, wi, weight, gloss);
}

uint BXDF_DisneyClearcoat_flags()
{
	return BXDF_FLAG_REFLECTION | BXDF_FLAG_GLOSSY;
}

#endif// BXDF_DISNEY_CLEARCOAT_GLSL
