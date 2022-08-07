//--------------------------------------------------
// Atta Resource Module
// texture.h
// Date: 2021-09-17
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_RESOURCE_RESOURCES_IMAGE_H
#define ATTA_RESOURCE_RESOURCES_IMAGE_H
#include <atta/memory/allocatedObject.h>
#include <atta/resource/resource.h>

namespace atta::resource {
class Image : public Resource, public AllocatedObject<Image, SID("ResourceAllocator")> {
  public:
    enum class Format {
        NONE = 0,
        RED8,
        RGB8,
        RGBA8,
        RGB16F,
    };

    struct CreateInfo {
        uint32_t width = 100;
        uint32_t height = 100;
        Format format = Format::RGBA8;
    };

    Image(const fs::path& filename);
    Image(const fs::path& filename, CreateInfo info);
    ~Image();

    void resize(uint32_t width, uint32_t height);
    void update();
    void saveToFile();

    uint32_t getWidth() const { return _width; }
    uint32_t getHeight() const { return _height; }
    uint32_t getChannels() const { return _channels; }
    Format getFormat() const { return _format; }

    static uint32_t getBytesPerChannel(Format format);

  private:
    void load();

    uint32_t _width;
    uint32_t _height;
    uint32_t _channels;
    uint32_t _bytesPerChannel;
    Format _format;
};
} // namespace atta::resource

#endif // ATTA_RESOURCE_RESOURCES_IMAGE_H
