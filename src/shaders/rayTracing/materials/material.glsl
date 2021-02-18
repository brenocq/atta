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
#include "mirror.glsl"
#include "metal.glsl"

BXDF Material_computeScatteringFunctions(inout Material material, vec2 uv)
{
	switch(material.type[0])
	{
		case MATERIAL_TYPE_DIFFUSE:
			return Material_Diffuse_computeScatteringFunctions(material);
		case MATERIAL_TYPE_MIRROR:
			return Material_Mirror_computeScatteringFunctions(material);
		case MATERIAL_TYPE_METAL:
			return Material_Metal_computeScatteringFunctions(material, uv);
	}

	BXDF bxdf;
	bxdf.type = BXDF_TYPE_NONE;
	return bxdf;
}

#endif// MATERIALS_MATERIAL_GLSL
