//--------------------------------------------------
// Atta Resource System
// texture.cpp
// Date: 2021-09-17
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/resourceSystem/resources/texture.h>
#include <atta/fileSystem/fileManager.h>

#include <atta/eventSystem/eventManager.h>
#include <atta/eventSystem/events/textureUpdateEvent.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

namespace atta
{
    Texture::Texture(const fs::path& filename):
        Resource(filename)
    {
        loadTexture();
    }

    Texture::Texture(const fs::path& filename, CreateInfo info):
        Resource(filename)
    {
        if(info.format == Format::NONE)
        {
            LOG_ERROR("Texture", "Trying to create texture resource [w]$0[] with format NONE, no texture will be create", filename.string());
            return;
        }

        if(info.width == 0)
        {
            LOG_ERROR("Texture", "Trying to create texture resource [w]$0[] with width=0, no texture will be create", filename.string());
            return;
        }

        if(info.height == 0)
        {
            LOG_ERROR("Texture", "Trying to create texture resource [w]$0[] with height=0, no texture will be create", filename.string());
            return;
        }

        _width = info.width;
        _height = info.height;
        _format = info.format;

        switch(_format)
        {
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

        _data = new uint8_t[_width*_height*_channels*getBytesPerChannel(_format)];
    }

    Texture::~Texture()
    {
        delete[] _data;
    }

    void Texture::resize(uint32_t width, uint32_t height)
    {
        delete[] _data;
        _width = width;
        _height = height;
        _data = new uint8_t[_width*_height*_channels*getBytesPerChannel(_format)];

        update();
    }

    void Texture::update()
    {
        TextureUpdateEvent e(_id);
        EventManager::publish(e);
    }

    uint32_t Texture::getBytesPerChannel(Format format)
    {
        return format == Format::RGB16F ? 2 : 1;
    }

    void Texture::loadTexture()
    {
        fs::path absolutePath = FileManager::solveResourcePath(_filename);
        std::string extension = _filename.extension().string();

        uint8_t* data = nullptr;
        int width, height, channels;
        if(extension != ".hdr")
        {
            data = stbi_load(absolutePath.string().c_str(), &width, &height, &channels, 0); 
        }
        else
        {
            stbi_set_flip_vertically_on_load(true);
            data = reinterpret_cast<uint8_t*>(stbi_loadf(absolutePath.string().c_str(), &width, &height, &channels, 0)); 
            stbi_set_flip_vertically_on_load(false);
        }

        if(data)
        {
            _width = width;
            _height = height;
            _channels = channels;

            if(extension != ".hdr")
            {
                if(_channels == 1)
                    _format = Format::RED8;
                else if(_channels == 3)
                    _format = Format::RGB8;
                else if(_channels == 4)
                    _format = Format::RGBA8;
                else
                    LOG_WARN("Texture", "Texture with $0 channels are not supported", _channels);
            }
            else
            {
                if(_channels != 3)
                    LOG_WARN("Texture", "Only hdr with 3 channels are supported");

                _format = Format::RGB16F;
            }

            // Copy temp data to _data
            uint32_t bytesPerPixel = _format == Format::RGB16F ? 2 : 1;
            uint32_t size = _width*_height*_channels*bytesPerPixel;
            _data = new uint8_t[size];
            for(int i = 0; i < size; i++)
                _data[i] = data[i];
            stbi_image_free(data);
            //LOG_WARN("Texture", "$3 -> w:$0, h:$1, c:$2", _width, _height, _channels, absolutePath);
        }
        else
        {
            _width = 0;
            _height = 0;
            _channels = 0;
            _format = Format::NONE;
            LOG_ERROR("Texture", "Failed to load texture [w]$0[]", absolutePath);
        }
    }
}

