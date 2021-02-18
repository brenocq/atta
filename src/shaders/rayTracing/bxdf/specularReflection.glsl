//--------------------------------------------------
// Atta Ray Tracing Vulkan GLSL
// specularReflection.glsl
// Date: 2021-02-14
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef BXDF_SPECULAR_REFLECTION_GLSL
#define BXDF_SPECULAR_REFLECTION_GLSL
#include "base.glsl"
#include "fresnel.glsl"

vec3 BXDF_SpecularReflection_sampleF(
		vec3 wo, out vec3 wi, vec2 u, out float pdf, 
		Fresnel fresnel, vec3 R)
{
	// Compute perfect specular reflection direction
	wi = vec3(-wo.x, -wo.y, wo.z);
	pdf = 1;

	return BXDF_Fresnel_evaluate(fresnel, cosTheta(wi)) * R/absCosTheta(wi);
}

uint BXDF_SpecularReflection_flags()
{
	return BXDF_FLAG_REFLECTION | BXDF_FLAG_SPECULAR;
}

#endif// BXDF_SPECULAR_REFLECTION_GLSL
