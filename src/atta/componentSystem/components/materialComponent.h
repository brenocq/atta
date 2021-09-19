//--------------------------------------------------
// Atta Component System
// materialComponent.h
// Date: 2021-09-18
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_COMPONENT_SYSTEM_COMPONENTS_MATERIAL_COMPONENT_H
#define ATTA_COMPONENT_SYSTEM_COMPONENTS_MATERIAL_COMPONENT_H
#include <atta/core/math/math.h>

namespace atta
{
	struct MaterialComponent
	{
		vec3 albedo = vec3(1.0f, 0.0f, 1.0f);
		float metallic = 0.0f;
		float roughness = 0.5f;
		float ao = 1.0f;
	};
}

#endif// ATTA_COMPONENT_SYSTEM_COMPONENTS_MATERIAL_COMPONENT_H
