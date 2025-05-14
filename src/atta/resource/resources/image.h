//--------------------------------------------------
// Atta Resource Module
// image.h
// Date: 2021-09-17
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_RESOURCE_RESOURCES_IMAGE_H
#define ATTA_RESOURCE_RESOURCES_IMAGE_H

#include <atta/memory/allocatedObject.h>
#include <atta/resource/resource.h>

namespace atta::resource {

class Image : public Resource, public memory::AllocatedObject<Image, SID("ResourceAllocator")> {
  public:
    enum class Format {
        NONE = 0,
        RED8,
        RGB8,
        RGBA8,
        RGB32F,
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

    /// Trigger update event to notify graphics module to update the image in the GPU
    void update();

    void saveToFile();

    // TODO get ImGui image reference

    uint32_t getWidth() const { return _width; }
    uint32_t getHeight() const { return _height; }
    uint32_t getChannels() const { return _channels; }
    Format getFormat() const { return _format; }

    /**
     * @brief Load image from file
     *
     * The image data is loaded from from the specified file, and written to _data.
     * The image _format, _width, _height and _channels are set according to the loaded image.
     *
     * If there is an error during the loading process, the image data will not be updated.
     *
     * @param filename The file to load the image from. If not specified, the filename of the resource is used.
     * @param triggerUpdate If true, the image will be updated in the GPU after loading.
     */
    void load(const fs::path& filename, bool triggerUpdate = true);

    static uint32_t getBytesPerChannel(Format format);
    static bool isFloatFormat(Format format);
    static const std::unordered_map<Format, std::string> formatToString;
    static const std::unordered_map<std::string, Format> stringToFormat;

  private:
    uint32_t _width = 0;
    uint32_t _height = 0;
    uint32_t _channels = 0;
    uint32_t _bytesPerChannel = 0;
    Format _format = Format::NONE;
};

} // namespace atta::resource

#endif // ATTA_RESOURCE_RESOURCES_IMAGE_H
