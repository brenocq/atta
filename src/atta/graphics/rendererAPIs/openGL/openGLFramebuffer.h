//--------------------------------------------------
// Atta Graphics Module
// openGLFramebuffer.h
// Date: 2021-09-09
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_GRAPHICS_RENDERER_APIS_OPENGL_OPENGL_FRAMEBUFFER_H
#define ATTA_GRAPHICS_RENDERER_APIS_OPENGL_OPENGL_FRAMEBUFFER_H

#include <atta/graphics/framebuffer.h>
#include <atta/graphics/rendererAPIs/openGL/base.h>

namespace atta::graphics {

class OpenGLFramebuffer final : public Framebuffer {
  public:
    OpenGLFramebuffer(const Framebuffer::CreateInfo& info);
    ~OpenGLFramebuffer();

    void bind(bool clear = true) override;
    void unbind() override;

    void resize(uint32_t width, uint32_t height, bool forceRecreate = false) override;

    int readPixel(unsigned attachmentIndex, unsigned x, unsigned y) override;

    /**
     * @brief Request image to be transferred from GPU to CPU
     *
     * It can be used to be able to do other processing while transferring the data to the CPU. To read the transferred image use the readImage method.
     *
     * @warning After making a request for an attachment index, you must call readImage for that attachment index before making another request.
     *
     * @param attachmentIndex Attachment index
     */
    void readImageRequest(unsigned attachmentIndex) override;
    std::vector<uint8_t> readImage(unsigned attachmentIndex) override;

    static GLenum convertDepthAttachmentType(Image::Format format);

  private:
    void createAttachments();
    void bindAttachments();

    /// Framebuffer object
    OpenGLId _id;

    // Pixel buffer object
    /**
     * @brief Pixel Buffer Object (PBO) id
     * It is used to asynchronously read the image
     */
    OpenGLId _pboId;

    /**
     * @brief PBO requested
     * If read image request was performed
     */
    bool _pboRequested;

    // Attachments
    int _depthAttachmentIndex;
    int _stencilAttachmentIndex;
};

} // namespace atta::graphics

#endif // ATTA_GRAPHICS_RENDERER_APIS_OPENGL_OPENGL_FRAMEBUFFER_H
