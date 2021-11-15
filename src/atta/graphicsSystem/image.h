//--------------------------------------------------
// Atta Graphics System
// image.h
// Date: 2021-09-09
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_GRAPHICS_SYSTEM_IMAGE_H
#define ATTA_GRAPHICS_SYSTEM_IMAGE_H
#include <atta/core/stringId.h>

namespace atta
{
    class Image
    {
    public:
        enum class Format
        {
            NONE = 0,
            RED,// 1 byte
            RGB,// 3 bytes
            RGBA,// 4 bytes
            RED32I,// 4 byte

            RGB16F,
            DEPTH32F,
            DEPTH24_STENCIL8,
        };

        enum class Wrap
        {
            NONE = 0,
            CLAMP,
            REPEAT
        };

        struct CreateInfo
        {
            Format format = Format::RGBA;
            Wrap samplerWrap = Wrap::REPEAT;
            uint32_t width = 1;
            uint32_t height = 1;
            uint32_t mipLevels = 1;
            StringId debugName = StringId("Unnamed Image");
            uint8_t* data = nullptr;
        };

        Image(const CreateInfo& info);
        virtual ~Image() = default;

        virtual void write(void* data) = 0;

        Format getFormat() const { return _format; }
        Wrap getSamplerWrap() const { return _samplerWrap; }
        uint32_t getWidth() const { return _width; }
        uint32_t getHeight() const { return _height; }
        float getRatio() const { return _width/static_cast<float>(_height); }
        virtual void* getImGuiImage() = 0;

        static uint32_t getFormatSize(Format format);
        static bool isDepthFormat(Format format);
        static bool isStencilFormat(Format format);

    protected:
        Format _format;
        Wrap _samplerWrap;
        uint32_t _width;
        uint32_t _height;
        uint32_t _mipLevels;

        const StringId _debugName;
    };
}

#endif// ATTA_GRAPHICS_SYSTEM_IMAGE_H
