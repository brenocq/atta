//--------------------------------------------------
// Atta Component Module
// dataManager.h
// Date: 2023-05-26
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_COMPONENT_DATA_MANAGER_DATA_MANAGER_H
#define ATTA_COMPONENT_DATA_MANAGER_DATA_MANAGER_H
#include <atta/component/components/component.h>

namespace atta::component {

class DataManager {
  public:
    /**
     * @brief Maximum number of components that can be registered
     *
     * If more components than this are registered, it is not possible to track if that component was allocated by using the EntityTable
     */
    constexpr unsigned maxComponents = 32;

    /**
     * @brief Maximum number of entities that can be created
     *
     * TODO make it dynamic
     */
    constexpr unsigned maxEntities = 1024;

    /**
     * @brief Entity Table
     *
     * The entity table has one pointer for each registered component. The pointer is nullptr if the component was not allocated for this entity
     */
    struct EntityTable {
        Component* components[maxRegisteredComponents];
    };

    /**
     * @brief Component Pool
     *
     * Bitmap pool allocator to keep track of which components are allocated
     */
    struct ComponentPool {
        uint8_t* memory;        ///< Pointer to first allocated component
        uint32_t size;          ///< Size in bytes of the whole pool
        uint32_t bitmapSize;    ///< Size in bytes of the bitmap section
        uint32_t componentSize; ///< Size in bytes of the component
        uint32_t current;       ///< Position to allocate next component
    };

    /**
     * @brief Add component to entity
     *
     * @param entity Entity to add the component to
     * @param cid Component id
     *
     * @return Component pointer
     */
    ATTA_CPU_GPU uint8_t* addComponent(Entity entity, ComponentId cid);

    /**
     * @brief Remove component from entity
     *
     * @param entity Entity to remove the component from
     * @param cid Component id
     */
    ATTA_CPU_GPU void removeComponent(Entity entity, ComponentId cid);

    /**
     * @brief Get entity component
     *
     * @param entity Entity to get component from
     * @param cid Component id
     *
     * @return Component pointer
     */
    ATTA_CPU_GPU uint8_t* getComponent(Entity entity, ComponentId cid);

    /**
     * @brief Create entity
     *
     * @param entity Entity id
     *
     * @return Entity
     *
     * Can try to create entity with specific EntityId. If none is provided, or if it is not possible to allocate with a certain id, the bitmap pool
     * will choose the id.
     */
    ATTA_CPU_GPU Entity createEntity(EntityId entity = -1);

    /**
     * @brief Destroy entity and deallocate components
     *
     * @param entity Entity to delete
     */
    ATTA_CPU_GPU void destroyEntity(Entity entity);

  protected:
    std::array<EntityTable, maxEntities> _entityPool;
    std::array<ComponentPool, maxComponents> _componentPools;
};

} // namespace atta::component

#endif // ATTA_COMPONENT_DATA_MANAGER_DATA_MANAGER_H
