//--------------------------------------------------
// Atta Graphics
// objectInfo.h
// Date: 2021-02-14
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_GRAPHICS_OBJECT_INFO_H
#define ATTA_GRAPHICS_OBJECT_INFO_H

#include "simulator/math/math.h"

namespace atta
{
	struct ObjectInfo
	{
		alignas(4) int indexOffset;
		alignas(4) int vertexOffset;
		alignas(4) int materialOffset;
		alignas(16) mat4 transform;

		std::string toString()
		{
			return std::string("ObjectInfo{")+
				"indexOffset="+std::to_string(indexOffset)+
				", vertexOffset="+std::to_string(vertexOffset)+
				", materialOffset="+std::to_string(materialOffset)+
				"}";
		}
	};
}

#endif// ATTA_GRAPHICS_OBJECT_INFO_H
