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

vec3 BXDF_LambertianReflection_rho(vec3 R)
{
	return R;
}

uint BXDF_LambertianReflection_flags()
{
	return BXDF_FLAG_REFLECTION | BXDF_FLAG_DIFFUSE;
}

#endif// BXDF_LAMBERTIAN_REFLECTION_GLSL
