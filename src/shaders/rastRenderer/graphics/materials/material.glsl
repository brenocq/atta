//--------------------------------------------------
// Atta Rasterization Renderer GLSL
// material.glsl
// Date: 2021-03-04
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_RAST_MATERIALS_MATERIAL_GLSL
#define ATTA_RAST_MATERIALS_MATERIAL_GLSL
#include "../../material.glsl"
#include "diffuse.glsl"

vec3 Material_computeColor(Material material, vec2 uv)
{
	switch(material.type[0])
	{
		case MATERIAL_TYPE_DIFFUSE:
			return Material_Diffuse_computeColor(material, uv);
		case MATERIAL_TYPE_MIRROR:
		case MATERIAL_TYPE_METAL:
		case MATERIAL_TYPE_DISNEY:
			return vec3(1,0,1);
	}

	return vec3(1,0,1);
}

#endif// ATTA_RAST_MATERIALS_MATERIAL_GLSL
