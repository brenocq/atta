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
#define COMPONENT_POOL_SID(T) SID((std::string("Component_") + typeid(T).name() + "Allocator").c_str())
#define COMPONENT_POOL_SSID(T) SSID((std::string("Component_") + typeid(T).name() + "Allocator").c_str())
#define COMPONENT_POOL_SID_BY_NAME(typeidTname) SID((std::string("Component_") + typeidTname + "Allocator").c_str())
#define COMPONENT_POOL_SSID_BY_NAME(typeidTname) SSID((std::string("Component_") + typeidTname + "Allocator").c_str())

    using EntityId = int;// Index inside entity pool
    using ComponentId = StringHash;// Component allocator name hash (COMPONENT_POOL_SID(T) result)
}

#endif// ATTA_COMPONENT_SYSTEM_BASE_H
