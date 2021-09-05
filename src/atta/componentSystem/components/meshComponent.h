//--------------------------------------------------
// Atta Component System
// meshComponent.h
// Date: 2021-09-02
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_COMPONENT_SYSTEM_COMPONENTS_MESH_COMPONENT_H
#define ATTA_COMPONENT_SYSTEM_COMPONENTS_MESH_COMPONENT_H
#include <atta/core/math/math.h>

namespace atta
{
	struct MeshComponent
	{
		vec4 vertex;
		int index;
	};
}

#endif// ATTA_COMPONENT_SYSTEM_COMPONENTS_MESH_COMPONENT_H
