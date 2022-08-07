//--------------------------------------------------
// Atta Component Module
// scriptComponent.cpp
// Date: 2021-11-23
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/component/components/scriptComponent.h>

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
