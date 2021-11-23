//--------------------------------------------------
// Atta Component System
// scriptComponent.cpp
// Date: 2021-11-23
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/componentSystem/components/scriptComponent.h>

namespace atta
{
    template<>
    ComponentRegistry::Description& TypedComponentRegistry<ScriptComponent>::getDescription()
    {
        static ComponentRegistry::Description desc = 
            {
                "Script",
                {
                    { 
                        ComponentRegistry::AttributeType::STRINGID, 
                        offsetof(ScriptComponent, sid), 
                        "sid", 
                        {},
                        {},
                        {},
                        {}
                    },
                }
            };

        return desc;
    }
}
