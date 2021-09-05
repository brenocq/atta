//--------------------------------------------------
// Atta Component System
// transformComponent.h
// Date: 2021-09-02
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_COMPONENT_SYSTEM_COMPONENTS_TRANSFORM_COMPONENT_H
#define ATTA_COMPONENT_SYSTEM_COMPONENTS_TRANSFORM_COMPONENT_H
#include <atta/core/math/math.h>

namespace atta
{
	struct TransformComponent
	{
		mat4 transform;
	};
}

#endif// ATTA_COMPONENT_SYSTEM_COMPONENTS_TRANSFORM_COMPONENT_H
