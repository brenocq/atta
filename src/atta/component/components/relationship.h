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
    static void setParent(Entity parent, Entity child);
    /// Remove entity parent
    /** Has the same effect of removing a child with
     * Relationship::removeChild.
     */
    static void removeParent(Entity parent, Entity child);

    //----- Child operations -----//
    /// Add child entity
    /** Has the same effect of setting the child parent with
     * Relationship::setParent.
     * */
    static void addChild(Entity parent, Entity child);
    /// Remove child entity
    /** Has the same effect of removing the child parent with
     * Relationship::removeParent.
     * */
    static void removeChild(Entity parent, Entity child);

    // Get data
    Entity getParent() { return _parent; }
    std::vector<Entity> getChildren() { return _children; }
    Entity get(uint32_t i) { return _children[i]; }

    // Data
    Entity _parent = -1;
    std::vector<Entity> _children = {};
};
ATTA_REGISTER_COMPONENT(Relationship)
template <>
ComponentDescription& TypedRegistry<Relationship>::getDescription();

} // namespace atta::component

#endif // ATTA_COMPONENT_COMPONENTS_RELATIONSHIP_H
