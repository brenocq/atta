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

BXDF Material_Diffuse_computeScatteringFunctions(inout Material material)
{
	BXDF bxdf;
	bxdf.type = BXDF_TYPE_NONE;

	vec3 Kd = material.datav[0].xyz;
	float sigma = material.dataf[0];

	if(!isBlack(Kd))// If not black
	{
		if(sigma == 0)
		{
			// Add Lambertian
			bxdf.type = BXDF_TYPE_LAMBERTIAN_REFLECTION;	
			bxdf.datav[0] = Kd;// R
		}
		else
		{
			// Add Oren Nayar
			bxdf.type = BXDF_TYPE_OREN_NAYAR;
			bxdf.datav[0] = Kd;// R
			bxdf.dataf[0] = sigma;// Sigma
		}
	}
	return bxdf;
}

#endif// MATERIALS_DIFFUSE_GLSL
