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
    /**
     * @brief Alignment type to be used when building the buffer layout
     *
     * The default alignment type is the same used by the C++ standard, and it is used for buffers that can be tightly packed. The GLSL alignment type
     * is used mainly when sending data to shaders, they differ mainly by the VEC3/MAT3/MAT4 alignment
     *
     * @warning The alignment type should be set before pushing types
     */
    enum class AlignmentType { DEFAULT = 0, GLSL };

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
        static uint32_t sizeFromType(AlignmentType alignmentType, Type type);
        /// Type alignment in bytes
        static uint32_t alignmentFromType(AlignmentType alignmentType, Type type);
        /// Number of component in the type (FLOAT -> 1, VEC2 -> 2, MAT4 -> 16, ...)
        static uint32_t componentCountFromType(Type type);
    };

    BufferLayout(AlignmentType alignmentType = AlignmentType::DEFAULT);
    ~BufferLayout() = default;

    /// Set alignment type
    void setAlignmentType(AlignmentType alignmentType);

    /**
     * @brief Push element
     *
     * @note The element will not be pushed if the type is NONE, of if the name is duplicated
     * @note The custom alignment is useful when working with structs. The type alignment of the first struct element may differ from the struct
     * alignment
     *
     * @param type Element type
     * @param name Element name
     * @param align Custom element alignment, set to 0 to type alignment
     */
    void push(Element::Type type, std::string name, uint32_t customAlign = 0);

    /// Get elements
    const std::vector<Element>& getElements() const;

    /// Check if element exists in the buffer
    bool exists(std::string name) const;

    /// Get buffer alignment in bytes (maximum alignment from elements)
    uint32_t getAlignment() const;

    /// Get buffer stride in bytes (aligned sum of the size of all elements)
    uint32_t getStride() const;

    /// Check if buffer layout is empty (no elements)
    bool empty() const;

  private:
    AlignmentType _alignmentType;   ///< How the elements should be aligned in the buffer
    std::vector<Element> _elements; ///< Elements in the buffer
};

} // namespace atta::graphics

#endif // ATTA_GRAPHICS_BUFFER_LAYOUT_H
