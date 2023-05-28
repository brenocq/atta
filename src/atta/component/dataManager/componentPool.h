//--------------------------------------------------
// Atta Component Module
// componentPool.h
// Date: 2023-05-28
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_COMPONENT_DATA_MANAGER_COMPONENT_POOL_H
#define ATTA_COMPONENT_DATA_MANAGER_COMPONENT_POOL_H
#include <atta/component/base.h>
#include <atta/component/components/component.h>

namespace atta::component {

/**
 * @brief Component Pool
 *
 * Bitmap pool allocator to keep track of which components are allocated
 */
class ComponentPool {
  public:
    ComponentPool() = default;
    ComponentPool(uint8_t* memory_, uint8_t size_, uint32_t componentSize_, uint32_t numComponents_);

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

    /**
     * @brief Check if pool was already allocated
     *
     * @return True if pool was already allocated
     */
    ATTA_CPU bool isAllocated() const;

    ///< Get memory
    ATTA_CPU_GPU uint8_t* getMemory() const;

    ///< Get size
    ATTA_CPU_GPU uint32_t getSize() const;

    ///< Get component size in bytes
    ATTA_CPU_GPU uint32_t getComponentSize() const;

    ///< Get number of components
    ATTA_CPU_GPU uint32_t getNumComponents() const;

  private:
    /**
     * @brief Get bitmap bit value
     *
     * @param index Component index
     * @return Bit value
     */
    ATTA_CPU_GPU bool getBit(uint64_t index);

    /**
     * @brief Set bitmap bit value
     *
     * @param index Component index
     * @param bit Bit value
     */
    ATTA_CPU_GPU void setBit(uint64_t index, bool bit);

    /**
     * @brief Get component pool index from component pointer
     *
     * @param block Component pointer
     *
     * @return Component pool index
     */
    ATTA_CPU_GPU uint32_t getIndex(void* component);

    /**
     * @brief Get component pointer from component pool index
     *
     * @param index Component pool index
     *
     * @return Component pointer
     */
    ATTA_CPU_GPU void* getBlock(uint32_t index);

    uint8_t* _memory;        ///< Pointer to pool
    uint32_t _size;          ///< Size in bytes of the whole pool
    uint8_t* _dataStart;     ///< Pointer to first allocated component
    uint32_t _componentSize; ///< Size in bytes of the component
    uint32_t _numComponents; ///< Maximum number of components in the pool
    uint32_t _current;       ///< Position to allocate next component
};

} // namespace atta::component

#endif // ATTA_COMPONENT_DATA_MANAGER_COMPONENT_POOL_H
