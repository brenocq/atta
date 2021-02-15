//--------------------------------------------------
// Atta Ray Tracing Vulkan GLSL
// material.glsl
// Date: 2021-02-14
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef MATERIALS_MATERIAL_GLSL
#define MATERIALS_MATERIAL_GLSL
#include "base.glsl"
#include "../bxdf/base.glsl"
#include "diffuse.glsl"

BXDF Material_computeScatteringFunctions(inout Material material)
{
	BXDF bxdf;
	bxdf.type = BXDF_TYPE_NONE;
	switch(material.type[0])
	{
		case MATERIAL_TYPE_DIFFUSE:
				bxdf = Material_Diffuse_computeScatteringFunctions(material);
		//case MATERIAL_TYPE_MIRROR:
		//	{
		//		
		//	}
		//default:
	}

	return bxdf;
}

#endif// MATERIALS_MATERIAL_GLSL
