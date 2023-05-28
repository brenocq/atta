//--------------------------------------------------
// Atta Component Module
// dataManager.h
// Date: 2023-05-26
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_COMPONENT_DATA_MANAGER_DATA_MANAGER_H
#define ATTA_COMPONENT_DATA_MANAGER_DATA_MANAGER_H
#include <atta/component/base.h>
#include <atta/component/components/component.h>

namespace atta::component {

class DataManager {
  public:
    /**
     * @brief Maximum number of components that can be registered
     *
     * If more components than this are registered, it is not possible to track if that component was allocated by using the EntityTable
     */
    static constexpr unsigned maxComponents = 32;

    /**
     * @brief Maximum number of entities that can be created
     *
     * TODO make it dynamic
     */
    static constexpr unsigned maxEntities = 1024;

    /**
     * @brief Entity block
     *
     * The component table has one pointer for each registered component. The pointer is nullptr if the component was not allocated for this entity
     */
    struct EntityBlock {
        bool exist;                                       ///< Flag to define if entity exists
        std::array<Component*, maxComponents> components; ///< Component table

        Component*& operator[](size_t index);
        const Component* operator[](size_t index) const;
    };

    /**
     * @brief Component Pool
     *
     * Bitmap pool allocator to keep track of which components are allocated
     */
    struct ComponentPool {
        ComponentPool() = default;
        ComponentPool(uint8_t* memory_, uint8_t size_, uint32_t componentSize_, uint32_t numComponents_);

        uint8_t* memory;        ///< Pointer to first allocated component
        uint32_t size;          ///< Size in bytes of the whole pool
        uint32_t bitmapSize;    ///< Size in bytes of the bitmap section
        uint32_t componentSize; ///< Size in bytes of the component
        uint32_t current;       ///< Position to allocate next component

        /**
         * @brief Check if pool was already allocated
         *
         * @return True if pool was already allocated
         */
        ATTA_CPU bool isAllocated() const;

        /**
         * @brief Calculate number of bytes in the pool given the number of components and size of each component
         *
         * @param numComponents Number of components
         * @param componentSize Component size in bytes
         *
         * The pool size is given by the size to store the components plus the size to store the bitmap to keep track of which
         * components are allocated
         *
         * @return Total pool size in bytes
         */
        ATTA_CPU static uint32_t calcPoolSize(uint32_t numComponents, uint32_t componentSize);

        /**
         * @brief Calculate number of bytes in the bitmap section
         *
         * @param numComponents Number of components
         * @param componentSize Component size in bytes
         *
         * @return Bitmap section size in bytes
         */
        ATTA_CPU static uint32_t calcBitmapSize(uint32_t numComponents, uint32_t componentSize);

        /**
         * @brief Reset pool data to default
         */
        ATTA_CPU void reset();

        /**
         * @brief Allocate component
         *
         * @return Allocated component
         *
         * @note nullptr is returned if allocation is not possible
         */
        ATTA_CPU_GPU Component* alloc();

        /**
         * @brief Free component
         *
         * @param ptr Pointer to component to free
         */
        ATTA_CPU_GPU void free(Component* ptr);
    };

    /**
     * @brief Create entity
     *
     * @param eid Entity id
     *
     * @return Entity
     *
     * Can try to create entity with specific EntityId. If none is provided, or if it is not possible to allocate with a certain id, the bitmap pool
     * will choose the id.
     */
    ATTA_CPU_GPU EntityId createEntity(EntityId eid = -1);

    /**
     * @brief Destroy entity and deallocate components
     *
     * @param eid Entity to delete
     */
    ATTA_CPU_GPU void destroyEntity(EntityId eid);

    /**
     * @brief Check if entity exists
     *
     * @param eid Entity to check
     * @return True if entity exists
     */
    ATTA_CPU_GPU bool entityExists(EntityId eid);

    /**
     * @brief Add component to entity
     *
     * @param eid Entity to add the component to
     * @param cid Component id
     *
     * @return Component pointer
     */
    ATTA_CPU_GPU Component* addComponent(EntityId eid, ComponentId cid);

    /**
     * @brief Remove component from entity
     *
     * @param eid Entity to remove the component from
     * @param cid Component id
     */
    ATTA_CPU_GPU void removeComponent(EntityId eid, ComponentId cid);

    /**
     * @brief Get entity component
     *
     * @param eid Entity to get component from
     * @param cid Component id
     *
     * @return Component pointer
     */
    ATTA_CPU_GPU Component* getComponent(EntityId eid, ComponentId cid);

  protected:
    void initEntityPool();

    EntityId _currentEntity;                                  ///< Current position to create entity
    std::array<EntityBlock, maxEntities> _entityPool;         ///< Entity pool
    std::array<ComponentPool, maxComponents> _componentPools; ///< Components pools

  private:
    /**
     * @brief Check if entity id is valid
     *
     * @param entity Entity id
     *
     * @return True if entity id is valid
     */
    ATTA_CPU_GPU bool validEntity(EntityId entity);

    /**
     * @brief Check if component id is valid
     *
     * @param component Component id
     *
     * @return True if component id is valid
     */
    ATTA_CPU_GPU bool validComponent(ComponentId component);
};

} // namespace atta::component

#endif // ATTA_COMPONENT_DATA_MANAGER_DATA_MANAGER_H
