//--------------------------------------------------
// Atta Component Module
// entity.cpp
// Date: 2021-09-04
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/component/interface.h>

#include <atta/component/components/prototype.h>
#include <atta/component/components/relationship.h>
#include <atta/component/entity.h>

namespace atta::component {

ATTA_CPU_GPU Entity::operator EntityId() const { return _id; }

ATTA_CPU_GPU bool Entity::exists() const { return _id >= 0; }
ATTA_CPU_GPU EntityId Entity::getId() const { return _id; }

ATTA_CPU_GPU bool Entity::isPrototype() const {
    if(get<component::Prototype>())
        return true;

    Entity p = getParent();
    if (p.exists() && p.isPrototype())
        return true;

    return false;
}

ATTA_CPU_GPU Entity Entity::getParent() const {
    auto r = get<component::Relationship>();
    if (r)
        return r->getParent();
    return -1;
}

ATTA_CPU_GPU std::vector<Entity> Entity::getChildren() const {
    auto r = get<component::Relationship>();
    if (r)
        return r->getChildren();
    return {};
}

ATTA_CPU_GPU Entity Entity::getChild(unsigned i) const {
    std::vector<Entity> children = getChildren();
    if (i < children.size())
        return children[i];
    return -1;
}

} // namespace atta::component
