//--------------------------------------------------
// Atta Resource Module
// image.cpp
// Date: 2021-09-17
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/file/manager.h>
#include <atta/resource/resources/image.h>

#include <atta/event/events/imageUpdate.h>
#include <atta/event/interface.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

namespace atta::resource {

Image::Image(const fs::path& filename) : Resource(filename) { load(); }

Image::Image(const fs::path& filename, CreateInfo info) : Resource(filename) {
    if (info.format == Format::NONE) {
        LOG_ERROR("resource::Image", "Trying to create image resource [w]$0[] with format NONE, no image will be create", filename.string());
        return;
    }

    if (info.width == 0) {
        LOG_ERROR("resource::Image", "Trying to create image resource [w]$0[] with width=0, no image will be create", filename.string());
        return;
    }

    if (info.height == 0) {
        LOG_ERROR("resource::Image", "Trying to create image resource [w]$0[] with height=0, no image will be create", filename.string());
        return;
    }

    _width = info.width;
    _height = info.height;
    _format = info.format;

    switch (_format) {
    case Format::RED8:
        _channels = 1;
        break;
    case Format::RGB8:
        _channels = 3;
        break;
    case Format::RGBA8:
        _channels = 4;
        break;
    case Format::RGB16F:
        _channels = 3;
        break;
    }

    _data = new uint8_t[_width * _height * _channels * getBytesPerChannel(_format)];
    for (int i = 0; i < _width * _height * _channels * getBytesPerChannel(_format); i++)
        _data[i] = 0;
}

Image::~Image() { delete[] _data; }

void Image::resize(uint32_t width, uint32_t height) {
    _width = width;
    _height = height;
    delete[] _data;
    _data = new uint8_t[_width * _height * _channels * getBytesPerChannel(_format)];

    update();
}

void Image::update() {
    event::ImageUpdate e(_id);
    event::publish(e);
}

void Image::saveToFile() {
    fs::path absolutePath = file::solveResourcePath(_filename, false);
    fs::create_directories(absolutePath.parent_path());

    // Save buffer as image
    if (absolutePath.extension() == fs::path(".jpeg") || absolutePath.extension() == fs::path(".jpg")) {
        LOG_DEBUG("resource::Image", "Saving [w]$0[] as JPG image", absolutePath.string());
        stbi_write_jpg(absolutePath.string().c_str(), _width, _height, _channels, _data, _channels * _width);
    } else if (absolutePath.extension() == fs::path(".png")) {
        LOG_DEBUG("resource::Image", "Saving [w]$0[] as PNG image", absolutePath.string());
        stbi_write_png(absolutePath.string().c_str(), _width, _height, _channels, _data, _channels * _width);
    } else
        LOG_WARN("resource::Image", "Unknown image extension '[w]$0[]' when trying to save [w]$1[]", absolutePath.extension().string(),
                 absolutePath.string());
}

uint32_t Image::getBytesPerChannel(Format format) { return format == Format::RGB16F ? 2 : 1; }

void Image::load() {
    fs::path absolutePath = file::solveResourcePath(_filename);
    std::string extension = _filename.extension().string();

    uint8_t* data = nullptr;
    int width, height, channels;
    if (extension != ".hdr") {
        data = stbi_load(absolutePath.string().c_str(), &width, &height, &channels, 0);
    } else {
        stbi_set_flip_vertically_on_load(true);
        data = reinterpret_cast<uint8_t*>(stbi_loadf(absolutePath.string().c_str(), &width, &height, &channels, 0));
        stbi_set_flip_vertically_on_load(false);
    }

    if (data) {
        _width = width;
        _height = height;
        _channels = channels;

        if (extension != ".hdr") {
            if (_channels == 1)
                _format = Format::RED8;
            else if (_channels == 3)
                _format = Format::RGB8;
            else if (_channels == 4)
                _format = Format::RGBA8;
            else
                LOG_WARN("resource::Image", "Image with $0 channels are not supported", _channels);
        } else {
            if (_channels != 3)
                LOG_WARN("resource::Image", "Only hdr with 3 channels are supported");

            _format = Format::RGB16F;
        }

        // Copy temp data to _data
        uint32_t bytesPerPixel = _format == Format::RGB16F ? 2 : 1;
        uint32_t size = _width * _height * _channels * bytesPerPixel;
        _data = new uint8_t[size];
        for (int i = 0; i < size; i++)
            _data[i] = data[i];
        stbi_image_free(data);
        // LOG_WARN("resource::Image", "$3 -> w:$0, h:$1, c:$2", _width, _height, _channels, absolutePath);
    } else {
        _width = 0;
        _height = 0;
        _channels = 0;
        _format = Format::NONE;
        LOG_ERROR("resource::Image", "Failed to load image [w]$0[]", absolutePath);
    }
}

} // namespace atta::resource
