//--------------------------------------------------
// Atta Graphics Module
// framebuffer.h
// Date: 2021-09-09
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_GRAPHICS_FRAMEBUFFER_H
#define ATTA_GRAPHICS_FRAMEBUFFER_H

#include <atta/graphics/image.h>
#include <atta/utils/math/vector.h>
#include <atta/utils/stringId.h>

namespace atta::graphics {

/// Framebuffer interface
/** This interface is implemented for each GraphicsAPI. The framebuffer have multiple attachments.
 * Usually one attachment is used for the output image, and the other to render the depth values.
 * But one framebuffer can have any number of attachments
 **/
class Framebuffer {
  public:
    /// Framebuffer attachment. Only the format or the image should be defined
    struct Attachment {
        /// Create image from format
        /** Can provide only the format to be faster. The image will be created when the framebuffer is created */
        Image::Format format = Image::Format::NONE;

        /// Use image that was already created
        /** Does not need to define the format when attaching an image directly. The (width, height) should be the same as the framebuffer **/
        std::shared_ptr<Image> image = nullptr;
    };

    struct CreateInfo {
        uint32_t width;                             ///< Framebuffer width
        uint32_t height;                            ///< Framebuffer height
        vec4 clearColor = {0.0f, 0.0f, 0.0f, 0.0f}; ///< Color to be used when cleaning the color attachments

        std::vector<Attachment> attachments = {};

        StringId debugName = StringId("Unnamed Framebuffer");
    };

    Framebuffer(const CreateInfo& info);
    virtual ~Framebuffer();

    virtual void bind(bool clear = true) = 0;
    virtual void unbind() = 0;

    virtual void resize(uint32_t width, uint32_t height, bool forceRecreate = false) = 0;

    std::vector<std::shared_ptr<Image>> getImages() const;
    std::shared_ptr<Image> getImage(uint32_t attachment = 0);
    uint32_t getWidth() const { return _width; }
    uint32_t getHeight() const { return _height; }
    vec4 getClearColor() const { return _clearColor; }
    StringId getDebugName() const { return _debugName; }

    bool hasColorAttachment() const;
    bool hasDepthAttachment() const;
    bool hasStencilAttachment() const;
    int getColorAttachmentIndex() const;
    int getDepthAttachmentIndex() const;
    int getStencilAttachmentIndex() const;

  protected:
    uint32_t _width;
    uint32_t _height;
    vec4 _clearColor;

    std::vector<Attachment> _attachments;
    int _colorAttachmentIndex;
    int _depthAttachmentIndex;
    int _stencilAttachmentIndex;
    std::vector<std::shared_ptr<Image>> _images;

    const StringId _debugName;
};

} // namespace atta::graphics

#endif // ATTA_GRAPHICS_FRAMEBUFFER_H
