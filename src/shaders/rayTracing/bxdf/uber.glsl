//--------------------------------------------------
// Atta Ray Tracing Vulkan GLSL
// uber.glsl
// Date: 2021-06-30
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef BXDF_UBER_GLSL
#define BXDF_UBER_GLSL
#include "../base.glsl"
#include "base.glsl"

vec3 BXDF_Uber_f(vec3 wo, vec3 wi, vec3 R, float sigma)
{
}

vec3 BXDF_Uber_sampleF(vec3 wo, out vec3 wi, vec2 u, out float pdf, 
		vec3 R, Fresnel fresnel, MicrofacetDistribution distribution)
{
	uint seed  = uint(u.x*10000);
	float random = randomFloat(seed);

	// Sample one BXDF
	int qtyBXDF = int(microfacetReflection)+int(microfacetTransmission)+
		int(clearCoat)+int(lambertianTransmission)+int(disneyRetro)+
		int(disneyDiffuse)+int(disneySheen)+int(disneyFakeSS);
	int count = min(int(random*qtyBXDF), qtyBXDF-1);

	// Sample specular transmission
	// Sample lambiertian reflection
	// Sample microfacet reflection
	// Sample specular transmission
}

uint BXDF_Uber_flags()
{
	return BXDF_FLAG_REFLECTION | BXDF_FLAG_DIFFUSE;
}

#endif// BXDF_UBER_GLSL
