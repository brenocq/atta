//--------------------------------------------------
// Atta Component System
// prototypeComponent.h
// Date: 2021-09-17
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_COMPONENT_SYSTEM_COMPONENTS_PROTOTYPE_COMPONENT_H
#define ATTA_COMPONENT_SYSTEM_COMPONENTS_PROTOTYPE_COMPONENT_H
#include <atta/componentSystem/components/component.h>
#include <atta/componentSystem/componentRegistry.h>

namespace atta
{
	/// %Component to create entity clones
	/** Every entity with this component is a protype. Prototypes are not simulated, 
	 * they are only used as template to create clone entities.
	 * 
	 * All components from the prototype entity are copied to the clones.
	 * Clone data are allocated so they are continuous in memory, this makes simulating
	 * clones faster than simulating entities scattered throughout the memory.
	 */
    struct PrototypeComponent final : public Component
    {
        uint64_t maxClones;///< Number of clones to be created
    };
    ATTA_REGISTER_COMPONENT(PrototypeComponent)
    template<> ComponentRegistry::Description& TypedComponentRegistry<PrototypeComponent>::getDescription();
}

#endif// ATTA_COMPONENT_SYSTEM_COMPONENTS_PROTOTYPE_COMPONENT_H
