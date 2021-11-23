//--------------------------------------------------
// Atta Component System
// nameComponent.cpp
// Date: 2021-11-23
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/componentSystem/components/nameComponent.h>

namespace atta
{
    template<>
    ComponentRegistry::Description& TypedComponentRegistry<NameComponent>::getDescription()
    {
        static ComponentRegistry::Description desc = 
            {
                "Name",
                {
                    { ComponentRegistry::AttributeType::CHAR, offsetof(NameComponent, name), "name" }
                }
            };

        return desc;
    }
}
