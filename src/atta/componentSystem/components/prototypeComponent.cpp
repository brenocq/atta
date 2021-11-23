//--------------------------------------------------
// Atta Component System
// prototypeComponent.cpp
// Date: 2021-11-23
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/componentSystem/components/prototypeComponent.h>

namespace atta
{
    template<>
    ComponentRegistry::Description& TypedComponentRegistry<PrototypeComponent>::getDescription()
    {
        static ComponentRegistry::Description desc = 
            {
                "Prototype",
                {
                    { ComponentRegistry::AttributeType::UINT64, offsetof(PrototypeComponent, maxClones), "maxClones", uint64_t(0), uint64_t(1000), 1 }
                }
            };

        return desc;
    }
}