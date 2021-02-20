//--------------------------------------------------
// Atta Ray Tracing Vulkan GLSL
// diffuse.glsl
// Date: 2021-02-13
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef MATERIALS_DIFFUSE_GLSL
#define MATERIALS_DIFFUSE_GLSL
#include "base.glsl"
#include "../base.glsl"
#include "../bxdf/base.glsl"

BSDF Material_Diffuse_computeScatteringFunctions(inout Material material)
{
	BSDF bsdf;
	bsdf.nBxdf = 1;

	vec3 Kd = material.datav[0].xyz;
	float sigma = material.dataf[0];

	if(!isBlack(Kd))// If not black
	{
		if(sigma == 0)
		{
			// Add Lambertian
			bsdf.bxdf[0].type = BXDF_TYPE_LAMBERTIAN_REFLECTION;	
			bsdf.bxdf[0].datav[0] = Kd;// R
		}
		else
		{
			// Add Oren Nayar
			bsdf.bxdf[0].type = BXDF_TYPE_OREN_NAYAR;
			bsdf.bxdf[0].datav[0] = Kd;// R
			bsdf.bxdf[0].dataf[0] = sigma;// Sigma
		}
	}
	return bsdf;
}

#endif// MATERIALS_DIFFUSE_GLSL
