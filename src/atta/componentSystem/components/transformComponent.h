//--------------------------------------------------
// Atta Component System
// transformComponent.h
// Date: 2021-09-02
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_COMPONENT_SYSTEM_COMPONENTS_TRANSFORM_COMPONENT_H
#define ATTA_COMPONENT_SYSTEM_COMPONENTS_TRANSFORM_COMPONENT_H
#include <atta/componentSystem/components/component.h>
#include <atta/componentSystem/componentRegistry.h>
#include <atta/core/math/math.h>

namespace atta
{
	/// Entity transform
	/** Define the entity position in the world, its orientation, and
	 * its scale.
	 * Only entities with a TransformComponent can be rendered or
	 * interact physically with other entities.
	 */
    struct TransformComponent final : public Component
    {
        vec3f position = vec3();///< Position
        quat orientation = quat();///< Orientation
        vec3f scale = vec3(1,1,1);///< Scale

		// Get transform matrix in world coordinate
		/** If the entity is child of another entity the 
		 * resulting transform matrix will depend on its
		 * parent world transform.
		 *
		 * The world transform will only differ from the local 
		 * transform when the entity also has a  RelationshipComponent.
		 */
        mat4 getWorldTransform(EntityId entity);

		// Get transform matrix in local coordinate
		/** Transform matrix calculated directly from position/orientation/scale. */
        mat4 getLocalTransform();
    };
    ATTA_REGISTER_COMPONENT(TransformComponent)
    template<> ComponentRegistry::Description& TypedComponentRegistry<TransformComponent>::getDescription();
}

#endif// ATTA_COMPONENT_SYSTEM_COMPONENTS_TRANSFORM_COMPONENT_H
