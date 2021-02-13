//--------------------------------------------------
// Atta Graphics
// material.h
// Date: 2021-01-08
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_GRAPHICS_MATERIAL_H
#define ATTA_GRAPHICS_MATERIAL_H

#include "simulator/math/math.h"

namespace atta
{
	struct Material
	{
		alignas(16) vec3 albedo = {1.0,1.0,1.0};
		alignas(4) float metallic = 0.5f;
		alignas(4) float roughness = 0.5f;
		alignas(4) float ao = 0.0;// Ambient occusion
		alignas(4) int albedoIndex = -1;
		alignas(4) int normalIndex = -1;// TODO get value from normal texture
		alignas(4) int metallicIndex = -1;// TODO get value from metallic texture
		alignas(4) int roughnessIndex = -1;// TODO get value from roughness texture
		alignas(4) int aoIndex = -1;// TODO get value from ao texture

		std::string toString()
		{
			return std::string("Material{albedo=")+albedo.toString()+
				", albedoIndex="+std::to_string(albedoIndex)+
				"}";
		}
	};
}

#endif// ATTA_GRAPHICS_MATERIAL_H
