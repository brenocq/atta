//--------------------------------------------------
// Atta Resource System
// texture.h
// Date: 2021-09-17
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_RESOURCE_SYSTEM_RESOURCES_TEXTURE_H
#define ATTA_RESOURCE_SYSTEM_RESOURCES_TEXTURE_H
#include <atta/resourceSystem/resource.h>
#include <atta/memorySystem/allocatedObject.h>

namespace atta
{
    class Texture : public Resource, public AllocatedObject<Texture, SID("ResourceAllocator")>
    {
    public:
        enum class Format
        {
            NONE = 0,
            RED8,
            RGB8,
            RGBA8,
            RGB16F,
        };

        struct CreateInfo
        {
            uint32_t width = 100;
            uint32_t height = 100;
            Format format = Format::RGBA8;
        };

        Texture(const fs::path& filename);
        Texture(const fs::path& filename, CreateInfo info);
        ~Texture();

        void resize(uint32_t width, uint32_t height);
        void update();

        uint32_t getWidth() const { return _width; }
        uint32_t getHeight() const { return _height; }
        uint32_t getChannels() const { return _channels; }
        Format getFormat() const { return _format; }

        static uint32_t getBytesPerChannel(Format format);

    private:
        void loadTexture();

        uint32_t _width;
        uint32_t _height;
        uint32_t _channels;
        uint32_t _bytesPerChannel;
        Format _format;
    };
}

#endif// ATTA_RESOURCE_SYSTEM_RESOURCES_TEXTURE_H
