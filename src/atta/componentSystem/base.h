//--------------------------------------------------
// Atta Component System
// base.h
// Date: 2021-09-17
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_COMPONENT_SYSTEM_BASE_H
#define ATTA_COMPONENT_SYSTEM_BASE_H
#include <atta/core/stringId.h>

namespace atta
{
	using EntityId = size_t;// Index inside entity pool
	using ComponentId = StringHash;// Component allocator name hash (COMPONENT_POOL_SID(T) result)
}

#endif// ATTA_COMPONENT_SYSTEM_BASE_H
