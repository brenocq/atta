//--------------------------------------------------
// Atta Graphics Module
// vertexBuffer.h
// Date: 2021-09-10
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_GRAPHICS_VERTEX_BUFFER_H
#define ATTA_GRAPHICS_VERTEX_BUFFER_H

namespace atta::graphics {

//---------- VertexBufferElement ----------//
class VertexBufferElement final {
  public:
    // TODO merge with type of shaderUniform.h
    enum class Type { NONE = 0, BOOL, INT, UINT, FLOAT, VEC2, VEC3, VEC4, IVEC2, IVEC3, IVEC4, MAT3, MAT4 };

    VertexBufferElement(std::string name_, Type type_, bool normalized_ = false);
    ~VertexBufferElement() = default;

    uint32_t getComponentCount() const;

    std::string name;
    Type type;
    uint32_t size;
    uint32_t offset;
    bool normalized;

  private:
    uint32_t sizeFromType(Type type) const;
};

//---------- VertexBufferLayout ----------//
class VertexBufferLayout final {
  public:
    VertexBufferLayout() = default;
    VertexBufferLayout(const std::initializer_list<VertexBufferElement>& elements);
    ~VertexBufferLayout() = default;

    const std::vector<VertexBufferElement>& getElements() const { return _elements; }
    uint32_t getElementCount() const { return static_cast<uint32_t>(_elements.size()); }
    uint32_t getStride() const { return _stride; }

  private:
    void calculateOffsetsAndStride();

    std::vector<VertexBufferElement> _elements;
    uint32_t _stride;
};

//---------- VertexBuffer ----------//
class VertexBuffer {
  public:
    enum class Usage {
        NONE,
        STATIC,
        DYNAMIC,
    };

    struct CreateInfo {
        VertexBufferLayout layout;     ///< Vertex buffer layout
        Usage usage = Usage::STATIC;   ///< Vertex buffer usage
        uint32_t size = 0;             ///< Size in bytes
        uint8_t* data = nullptr; ///< Vertex buffer data
    };

    VertexBuffer(const CreateInfo& info);
    virtual ~VertexBuffer() = default;

    virtual void bind() const = 0;

    uint32_t getSize() const { return _size; };
    VertexBufferLayout getLayout() const { return _layout; }

  protected:
    VertexBufferLayout _layout;
    Usage _usage;
    uint32_t _size;
    const uint8_t* _data;
};

} // namespace atta::graphics

#endif // ATTA_GRAPHICS_VERTEX_BUFFER_H
