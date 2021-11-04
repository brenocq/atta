//--------------------------------------------------
// Atta Component System
// scriptComponent.h
// Date: 2021-09-02
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_COMPONENT_SYSTEM_COMPONENTS_SCRIPT_COMPONENT_H
#define ATTA_COMPONENT_SYSTEM_COMPONENTS_SCRIPT_COMPONENT_H
#include <atta/componentSystem/components/component.h>

namespace atta
{
    struct ScriptComponent final : public TypedComponent<ScriptComponent>
    {
        StringId sid;
    };

    template<> const Component::TypeDescription TypedComponent<ScriptComponent>::typeDescription;
}

#endif// ATTA_COMPONENT_SYSTEM_COMPONENTS_SCRIPT_COMPONENT_H

