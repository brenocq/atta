//--------------------------------------------------
// Atta Ray Tracing Vulkan GLSL
// material.glsl
// Date: 2021-02-14
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef MATERIALS_MATERIAL_GLSL
#define MATERIALS_MATERIAL_GLSL
#include "base.glsl"
#include "matte.glsl"

void Material_computeScatteringFunctions(Material material)
{
	switch(material.type)
	{
		case MATERIAL_TYPE_MATTE:
			{
				vec3 Kd = vec3(0.5, 0.8, 0.7);
				float sigma = 20;
				//float bumpMap = 0; TODO implement bumpMap
				Material_Matte_computeScatteringFunctions(material, Kd, sigma);
			}
		//case MATERIAL_TYPE_MIRROR:
		//	{
		//		
		//	}
		//default:
	}
}

#endif// MATERIALS_MATERIAL_GLSL
