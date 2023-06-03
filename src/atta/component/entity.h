//--------------------------------------------------
// Atta Component Module
// entity.h
// Date: 2021-09-04
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_COMPONENT_ENTITY_H
#define ATTA_COMPONENT_ENTITY_H
#include <atta/component/base.h>

namespace atta::component {

class Entity {
  public:
    ATTA_CPU_GPU constexpr Entity() : _id(-1) {}
#if ATTA_GPU_CODE
    ATTA_GPU Entity(EntityId id) : _id(id) {}
#else
    ATTA_CPU constexpr Entity(EntityId id) : _id(id) {}
#endif

    ATTA_CPU_GPU operator EntityId() const;

    template <typename T>
    ATTA_CPU_GPU T* add() const;

    template <typename T>
    ATTA_CPU_GPU void remove() const;

    template <typename T>
    ATTA_CPU_GPU T* get() const;

    /// Check if entity exists
    ATTA_CPU_GPU bool exists() const;

    /// Get entity id
    ATTA_CPU_GPU EntityId getId() const;

    /// Check if entity is prototype
    /** An entity is prototype if it has a prototype component or if its parent is a prototype **/
    ATTA_CPU bool isPrototype() const;

    // Relationship component interface
    ATTA_CPU Entity getParent() const;
    ATTA_CPU std::vector<Entity> getChildren() const;
    ATTA_CPU Entity getChild(unsigned i) const;

  private:
    EntityId _id;
};

} // namespace atta::component

namespace std {
template <>
struct hash<atta::component::Entity> {
    size_t operator()(atta::component::Entity const& entity) const { return static_cast<size_t>(entity.getId()); }
};
} // namespace std

#include <atta/component/entity.inl>

#endif // ATTA_COMPONENT_ENTITY_H
