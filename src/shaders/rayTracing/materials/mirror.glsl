//--------------------------------------------------
// Atta Ray Tracing Vulkan GLSL
// mirror.glsl
// Date: 2021-02-17
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef MATERIALS_MIRROR_GLSL
#define MATERIALS_MIRROR_GLSL
#include "base.glsl"
#include "../base.glsl"
#include "../bxdf/base.glsl"
#include "../bxdf/fresnel.glsl"

BXDF Material_Mirror_computeScatteringFunctions(inout Material material)
{
	BXDF bxdf;

	// Add Specular reflection
	bxdf.type = BXDF_TYPE_SPECULAR_REFLECTION;	
	bxdf.datai[0] = BXDF_FRESNEL_TYPE_NOOP;
	bxdf.datav[0] = material.datav[0].xyz;

	return bxdf;
}

#endif// MATERIALS_MIRROR_GLSL
