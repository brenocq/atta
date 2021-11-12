//--------------------------------------------------
// Atta Graphics System
// renderer.h
// Date: 2021-09-14
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_GRAPHICS_SYSTEM_RENDERER_H
#define ATTA_GRAPHICS_SYSTEM_RENDERER_H
#include <atta/core/stringId.h>
#include <atta/graphicsSystem/framebuffer.h>
#include <atta/graphicsSystem/cameras/camera.h>
#include <atta/fileSystem/serializer/serializable.h>

namespace atta
{
    class Renderer : public Serializable
    {
    public:
        Renderer(const char* name): _name(StringId(name)) {}
        virtual ~Renderer() = default;

        virtual void render(std::shared_ptr<Camera> camera) = 0;
        virtual void resize(uint32_t width, uint32_t height) = 0;

        virtual uint32_t getWidth() const = 0;
        virtual uint32_t getHeight() const = 0;
        virtual void* getImGuiTexture() const = 0;

        std::string getName() const { return _name.getString(); }
        StringId getSID() const { return _name; }

        void serialize(std::ostream& os) override;
        void deserialize(std::istream& is) override;
        unsigned getSerializedSize() { return Serializable::getSerializedSize(this); }

    protected:
        uint32_t _width;
        uint32_t _height;
        StringId _name;
    };
}

#endif// ATTA_GRAPHICS_SYSTEM_RENDERER_H
