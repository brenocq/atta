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
    ComponentDescription& TypedComponentRegistry<ScriptComponent>::getDescription()
    {
        static ComponentDescription desc = 
            {
                "Script",
                {
                    { 
                        AttributeType::STRINGID, 
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
