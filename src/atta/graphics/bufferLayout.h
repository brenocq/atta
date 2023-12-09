//--------------------------------------------------
// Atta Graphics Module
// bufferLayout.h
// Date: 2023-08-30
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_GRAPHICS_BUFFER_LAYOUT_H
#define ATTA_GRAPHICS_BUFFER_LAYOUT_H

namespace atta::graphics {

class BufferLayout final {
  public:
    struct Element {
        enum class Type { NONE = 0, BOOL, INT, UINT, FLOAT, VEC2, VEC3, VEC4, IVEC2, IVEC3, IVEC4, MAT3, MAT4, SAMPLER_2D, SAMPLER_CUBE };

        Type type;
        std::string name;
        uint32_t offset;
        uint32_t size;

        /// Convert string to type ("float" -> FLOAT, "sampler2D" -> SAMPLER_2D, ...)
        static Type typeFromString(std::string type);
        /// Convert type to string
        static std::string typeToString(Type type);
        /// Type size in bytes
        static uint32_t sizeFromType(Type type);
        /// Type alignment in bytes
        static uint32_t alignmentFromType(Type type);
        /// Number of component in the type (FLOAT -> 1, VEC2 -> 2, MAT4 -> 16, ...)
        static uint32_t componentCountFromType(Type type);
    };

    BufferLayout() = default;
    BufferLayout(const std::initializer_list<Element>& elements);
    ~BufferLayout() = default;

    /**
     * @brief Push element
     *
     * @note The element will not be pushed if the type is NONE, of if the name is duplicated
     *
     * @param type Element type
     * @param name Element name
     */
    void push(Element::Type type, std::string name);
    /// Get elements
    const std::vector<Element>& getElements() const;
    /// Check if element exists in the buffer
    bool exists(std::string name) const;
    /// Get number of elements
    uint32_t getElementCount() const;
    /// Get buffer stride in bytes (aligned sum of the size of all elements)
    uint32_t getStride() const;

  private:
    std::vector<Element> _elements; ///< Elements in the buffer
};

} // namespace atta::graphics

#endif // ATTA_GRAPHICS_BUFFER_LAYOUT_H
