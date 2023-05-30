//--------------------------------------------------
// Atta Component Module
// entity.cpp
// Date: 2021-09-04
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/component/components/prototype.h>
#include <atta/component/components/relationship.h>
#include <atta/component/entity.h>
#include <atta/component/interface.h>

namespace atta::component {

ATTA_CPU_GPU Entity::operator EntityId() const { return _id; }

ATTA_CPU_GPU bool Entity::exists() const {
#if ATTA_GPU_CODE
    // return gpuDataManager->entityExists(_id);
    return false;
#else
    return cpuDataManager->entityExists(_id);
#endif
}

ATTA_CPU_GPU EntityId Entity::getId() const { return _id; }

ATTA_CPU_GPU bool Entity::isPrototype() const {
    if (get<Prototype>())
        return true;

    Entity p = getParent();
    if (p.exists() && p.isPrototype())
        return true;

    return false;
}

ATTA_CPU_GPU Entity Entity::getParent() const {
    auto r = get<Relationship>();
    if (r)
        return r->getParent();
    return -1;
}

ATTA_CPU std::vector<Entity> Entity::getChildren() const {
    auto r = get<Relationship>();
    if (r)
        return r->getChildren();
    return {};
}

ATTA_CPU Entity Entity::getChild(unsigned i) const {
    std::vector<Entity> children = getChildren();
    if (i < children.size())
        return children[i];
    return -1;
}

} // namespace atta::component
