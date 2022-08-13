//--------------------------------------------------
// Atta Component Module
// relationship.cpp
// Date: 2021-11-16
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/component/components/relationship.h>
#include <atta/component/components/transform.h>
#include <atta/component/interface.h>

namespace atta::component {
template <>
ComponentDescription& TypedComponentRegistry<Relationship>::getDescription() {
    static ComponentDescription desc = {
        "component::Relationship",
        {{AttributeType::UINT32, offsetof(Relationship, _parent), "parent"}, {AttributeType::CUSTOM, offsetof(Relationship, _children), "children"}},
        // Max instances
        1024,
        // Serialize
        {{"children",
          [](std::ostream& os, void* data) {
              std::vector<EntityId>* children = static_cast<std::vector<EntityId>*>(data);
              for (EntityId child : *children)
                  file::write(os, child);
              file::write(os, EntityId(-1));
          }}},
        // Deserialize
        {{"children", [](std::istream& is, void* data) {
              std::vector<EntityId>* children = static_cast<std::vector<EntityId>*>(data);
              EntityId eid;
              file::read(is, eid);
              while (eid != -1) {
                  children->push_back(eid);
                  file::read(is, eid);
              }
          }}}};

    return desc;
}

// Parent operations
void Relationship::setParent(EntityId parent, EntityId child) {
    // Get relationships
    Relationship* parentRel = component::getComponent<Relationship>(parent);
    if (!parentRel)
        parentRel = component::addComponent<Relationship>(parent);
    Relationship* childRel = component::getComponent<Relationship>(child);
    if (!childRel)
        childRel = component::addComponent<Relationship>(child);

    // Check if entity can be children (avoid recursive)
    bool canBeParent = true;
    Relationship* r = parentRel;
    if (r && r->_parent != -1) {
        // From the parent, going up in the hierarchy, can't find the child
        if (r->_parent == child)
            canBeParent = false;
        while (canBeParent && r != nullptr) {
            if (r->_parent == child)
                canBeParent = false;

            if (r->_parent == -1)
                r = nullptr;
            else
                r = component::getComponent<Relationship>(r->_parent);
        }
    }

    // Update relationship components
    if (canBeParent) {
        // Remove old parent
        if (childRel->_parent != -1)
            removeParent(childRel->_parent, child);

        // If has transform component, update to be relative to the parent
        Transform* t = component::getComponent<Transform>(child);
        if (t) {
            mat4 transform = t->getWorldTransform(child);
            mat4 pTransform = Transform::getEntityWorldTransform(parent);

            transform = inverse(pTransform) * transform;

            vec3 pos, scale;
            quat ori;
            transform.getPosOriScale(pos, ori, scale);

            t->position = pos;
            t->orientation = ori;
            t->scale = scale;
        }

        // Add new parent
        parentRel->_children.push_back(child);
        childRel->_parent = parent;
    }
}

void Relationship::removeParent(EntityId parent, EntityId child) {
    // Get relationships
    Relationship* parentRel = component::getComponent<Relationship>(parent);
    if (!parentRel && parent != -1) // Check if parent has relationship component
    {
        LOG_WARN("component::Relationship", "Trying to remove wrong parent [w]$0[] from child [w]$1[]", parent, child);
        return;
    }

    Relationship* childRel = component::getComponent<Relationship>(child);
    if (!childRel) // Check if child has relationship component
    {
        LOG_WARN("component::Relationship", "Trying to remove parent [w]$0[] from child [w]$1[] that does not have a parent", parent, child);
        return;
    }

    // If has transform component, change to be relative to the world
    Transform* transform = component::getComponent<Transform>(child);
    if (transform) {
        mat4 world = transform->getWorldTransform(child);
        vec3 pos, scale;
        quat ori;
        world.getPosOriScale(pos, ori, scale);

        transform->position = pos;
        transform->orientation = ori;
        transform->scale = scale;
    }

    childRel->_parent = -1;
    if (parentRel) {
        bool found = false;
        for (unsigned i = 0; i < parentRel->_children.size(); i++)
            if (parentRel->_children[i] == child) {
                parentRel->_children.erase(parentRel->_children.begin() + i);
                found = true;
                break;
            }
        if (!found)
            LOG_WARN("component::Relationship", "Removed wrong parent [w]$0[] from child [w]$1[]", parent, child);
    }
}

// Child operations
void Relationship::addChild(EntityId parent, EntityId child) { setParent(parent, child); }

void Relationship::removeChild(EntityId parent, EntityId child) { removeParent(parent, child); }

} // namespace atta::component
