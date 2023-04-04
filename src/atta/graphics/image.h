//--------------------------------------------------
// Atta Graphics Module
// image.h
// Date: 2021-09-09
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_GRAPHICS_IMAGE_H
#define ATTA_GRAPHICS_IMAGE_H

#include <atta/graphics/base.h>
#include <atta/utils/stringId.h>

namespace atta::graphics {

class Image {
  public:
    enum class Format {
        NONE = 0,
        RED,    // 1 byte
        RG16F,  // 4 bytes
        RGB,    // 3 bytes
        RGBA,   // 4 bytes
        RED32I, // 4 byte

        RGB16F,
        DEPTH32F,
        DEPTH24_STENCIL8,
    };

    enum class Wrap {
        NONE = 0,
        CLAMP,
        REPEAT,
        BORDER // Clamp to border
    };

    struct CreateInfo {
        Format format = Format::RGBA;
        Wrap samplerWrap = Wrap::REPEAT;
        /// Image border color
        /** Only used when samplerWrap is set to BORDER **/
        vec4 borderColor = vec4(1.0f);
        uint32_t width = 1;
        uint32_t height = 1;
        uint32_t mipLevels = 1;
        bool isCubemap = false;
        StringId debugName = StringId("Unnamed Image");
        /// Image data
        /** Can provide initialization data. If not provided, a black image will be created **/
        uint8_t* data = nullptr;
    };

    Image(const CreateInfo& info);
    virtual ~Image() = default;

    virtual void write(void* data) = 0;
    virtual void resize(uint32_t width, uint32_t height, bool forceRecreate = false) = 0;

    GfxId getId() const { return _id; }
    Format getFormat() const { return _format; }
    Wrap getSamplerWrap() const { return _samplerWrap; }
    uint32_t getWidth() const { return _width; }
    uint32_t getHeight() const { return _height; }
    float getRatio() const { return _width / static_cast<float>(_height); }
    bool isCubemap() const { return _isCubemap; }
    virtual void* getImGuiImage() = 0;

    static uint32_t getFormatSize(Format format);
    static bool isDepthFormat(Format format);
    static bool isStencilFormat(Format format);

  protected:
    GfxId _id;
    Format _format;
    Wrap _samplerWrap;
    vec4 _borderColor;
    uint32_t _width;
    uint32_t _height;
    uint32_t _mipLevels;
    bool _isCubemap;

    const StringId _debugName;
    uint8_t* _data;
};

} // namespace atta::graphics

#endif // ATTA_GRAPHICS_IMAGE_H
