//--------------------------------------------------
// Atta Component System
// scriptComponent.h
// Date: 2021-09-02
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_COMPONENT_SYSTEM_COMPONENTS_SCRIPT_COMPONENT_H
#define ATTA_COMPONENT_SYSTEM_COMPONENTS_SCRIPT_COMPONENT_H
#include <atta/componentSystem/components/component.h>
#include <atta/componentSystem/componentRegistry.h>
#include <atta/core/stringId.h>

namespace atta
{
    struct ScriptComponent final : public Component
    {
        StringId sid;
    };
    ATTA_REGISTER_COMPONENT(ScriptComponent)

    template<>
    inline const ComponentRegistry::Description TypedComponentRegistry<ScriptComponent>::description = 
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
                { "test" }
            },
        }
    };
}

#endif// ATTA_COMPONENT_SYSTEM_COMPONENTS_SCRIPT_COMPONENT_H

