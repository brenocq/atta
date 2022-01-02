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
    ComponentDescription& TypedComponentRegistry<NameComponent>::getDescription()
    {
        static ComponentDescription desc = 
            {
                "Name",
                {
                    { AttributeType::CHAR, offsetof(NameComponent, name), "name" }
                }
            };

        return desc;
    }
}
