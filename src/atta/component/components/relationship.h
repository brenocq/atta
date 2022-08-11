//--------------------------------------------------
// Atta Component Module
// relationship.h
// Date: 2021-09-24
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_COMPONENT_COMPONENTS_RELATIONSHIP_H
#define ATTA_COMPONENT_COMPONENTS_RELATIONSHIP_H

#include <atta/component/interface.h>
#include <atta/file/serializer/serializer.h>

namespace atta::component {

/// %Component to define relationship between entitities
/** Usually relationships are created by making one entity
 * child/parent of another entity using the entity tree.
 * Every entity that has a parent or a child has a
 * relationship component.
 *
 * To keep the hierarchy consistent, the relationship
 * component should only be changed using the available functions.
 * \ref _parent and \ref _children
 * should not be modified directly because this can result in breaking
 * other systems.
 *
 * The Transform is changed when the relationship component changes
 * so that the entity world transform stays the same
 */
struct Relationship final : public Component {
    //----- Parent operations -----//
    /// Change or set entity parent
    /** There is no need to remove the old parent before setting the new
     * parent.
     *
     * Has the same effect of adding a child with
     * Relationship::addChild.
     * */
    static void setParent(EntityId parent, EntityId child);
    /// Remove entity parent
    /** Has the same effect of removing a child with
     * Relationship::removeChild.
     */
    static void removeParent(EntityId parent, EntityId child);

    //----- Child operations -----//
    /// Add child entity
    /** Has the same effect of setting the child parent with
     * Relationship::setParent.
     * */
    static void addChild(EntityId parent, EntityId child);
    /// Remove child entity
    /** Has the same effect of removing the child parent with
     * Relationship::removeParent.
     * */
    static void removeChild(EntityId parent, EntityId child);

    // Get data
    EntityId getParent() { return _parent; }
    std::vector<EntityId> getChildren() { return _children; }

    // Data
    EntityId _parent = -1;
    std::vector<EntityId> _children;
};
ATTA_REGISTER_COMPONENT(Relationship)
template <>
ComponentDescription& TypedComponentRegistry<Relationship>::getDescription();

} // namespace atta::component

#endif // ATTA_COMPONENT_COMPONENTS_RELATIONSHIP_H
