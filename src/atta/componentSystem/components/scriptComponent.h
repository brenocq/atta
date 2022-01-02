//--------------------------------------------------
// Atta Component System
// scriptComponent.h
// Date: 2021-09-02
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_COMPONENT_SYSTEM_COMPONENTS_SCRIPT_COMPONENT_H
#define ATTA_COMPONENT_SYSTEM_COMPONENTS_SCRIPT_COMPONENT_H
#include <atta/componentSystem/componentManager.h>

namespace atta
{
	/// Script to be executed by the entity
	/** Each entity can only have one script attached to it.
	 * scripts are defined in the project folder and the script
	 * name is the cmake target name.
	 */
    struct ScriptComponent final : public Component
    {
		/** The script name is the same as the cmake target name */
        StringId sid;///< Script name
    };
    ATTA_REGISTER_COMPONENT(ScriptComponent)
    template<> ComponentDescription& TypedComponentRegistry<ScriptComponent>::getDescription();
}

#endif// ATTA_COMPONENT_SYSTEM_COMPONENTS_SCRIPT_COMPONENT_H
