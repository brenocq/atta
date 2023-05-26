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
ComponentDescription& TypedRegistry<Relationship>::getDescription() {
    static ComponentDescription desc = {
        "Relationship",
        {{AttributeType::UINT32, offsetof(Relationship, _parent), "parent"}, {AttributeType::CUSTOM, offsetof(Relationship, _children), "children"}},
        // Max instances
        1024,
        // Serialize
        {{"children",
          [](std::ostream& os, void* data) {
              std::vector<Entity>* children = static_cast<std::vector<Entity>*>(data);
              for (Entity child : *children)
                  file::write(os, EntityId(child));
              file::write(os, EntityId(-1));
          }}},
        // Deserialize
        {{"children", [](std::istream& is, void* data) {
              std::vector<Entity>* children = static_cast<std::vector<Entity>*>(data);
              EntityId eid;
#ifdef ATTA_OS_WEB
              file::read(is, eid); // For some reason the first one is always zero when building for the web
#endif
              file::read(is, eid);
              while (eid != -1) {
                  children->push_back(eid);
                  file::read(is, eid);
              }
          }}}};

    return desc;
}

// Parent operations
void Relationship::setParent(Entity parent, Entity child) {
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
            mat4 transform = t->getWorldTransformMatrix(child);
            Transform wt = Transform::getEntityWorldTransform(parent);
            mat4 pTransform;
            pTransform.setPosOriScale(wt.position, wt.orientation, wt.scale);

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

void Relationship::removeParent(Entity parent, Entity child) {
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
        mat4 world = transform->getWorldTransformMatrix(child);
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
void Relationship::addChild(Entity parent, Entity child) { setParent(parent, child); }

void Relationship::removeChild(Entity parent, Entity child) { removeParent(parent, child); }

} // namespace atta::component
