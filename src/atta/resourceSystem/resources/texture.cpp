//--------------------------------------------------
// Atta Resource System
// texture.cpp
// Date: 2021-09-17
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/resourceSystem/resources/texture.h>
#include <atta/fileSystem/fileManager.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

namespace atta
{
    Texture::Texture(const fs::path& filename):
        Resource(filename)
    {
        loadTexture();
    }

    Texture::~Texture()
    {
        stbi_image_free(_data);
    }

    void Texture::loadTexture()
    {
        fs::path absolutePath = FileManager::solveResourcePath(_filename);
        std::string extension = _filename.extension().string();

        int width, height, channels;
        if(extension != ".hdr")
            _data = stbi_load(absolutePath.string().c_str(), &width, &height, &channels, 0); 
        else
            _data = reinterpret_cast<uint8_t*>(stbi_loadf(absolutePath.string().c_str(), &width, &height, &channels, 0)); 

        if(_data)
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
            LOG_WARN("Texture", "$3 -> w:$0, h:$1, c:$2", _width, _height, _channels, absolutePath);
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

