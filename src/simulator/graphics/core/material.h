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
		vec3 albedo = {1.0,1.0,1.0};
		float metallic = 0.5f;
		float roughness = 0.5f;
		float ao = 0.0;// Ambient occusion
		int albedoIndex = -1;// TODO get value from albedo texture
		int normalIndex = -1;// TODO get value from normal texture
		int metallicIndex = -1;// TODO get value from metallic texture
		int roughnessIndex = -1;// TODO get value from roughness texture
		int aoIndex = -1;// TODO get value from ao texture
	};
}

#endif// ATTA_GRAPHICS_MATERIAL_H
