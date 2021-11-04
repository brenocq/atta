//--------------------------------------------------
// Atta Component System
// scriptComponent.cpp
// Date: 2021-11-04
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/componentSystem/components/scriptComponent.h>

namespace atta
{
    template<>
    const Component::TypeDescription TypedComponent<ScriptComponent>::typeDescription = 
    {
        Component::Type("Script"),
        {
            { 
                .type = Component::AttributeType::STRINGID, 
                .offset = attributeOffset(&ScriptComponent::sid), 
                .name = "sid", 
                .min = {},
                .max = {},
                .step = {},
                .options = { "test" }
            },
        }
    };
}

