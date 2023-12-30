//--------------------------------------------------
// Atta Graphics Module
// vertexBuffer.h
// Date: 2021-09-10
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_GRAPHICS_VERTEX_BUFFER_H
#define ATTA_GRAPHICS_VERTEX_BUFFER_H
#include <atta/graphics/bufferLayout.h>

namespace atta::graphics {

//---------- VertexBuffer ----------//
class VertexBuffer {
  public:
    enum class Usage {
        NONE,
        STATIC,
        DYNAMIC,
    };

    struct CreateInfo {
        BufferLayout layout;         ///< Vertex buffer layout
        Usage usage = Usage::STATIC; ///< Vertex buffer usage
        uint32_t size = 0;           ///< Size in bytes
        uint8_t* data = nullptr;     ///< Vertex buffer data
    };

    VertexBuffer(const CreateInfo& info);
    virtual ~VertexBuffer() = default;

    virtual void bind() const = 0;

    uint32_t getSize() const { return _size; };
    BufferLayout getLayout() const { return _layout; }

    /// Get number of vertices stored
    uint32_t getCount() const;

  protected:
    BufferLayout _layout;
    Usage _usage;
    uint32_t _size;
    const uint8_t* _data;
};

} // namespace atta::graphics

#endif // ATTA_GRAPHICS_VERTEX_BUFFER_H
