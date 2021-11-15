//--------------------------------------------------
// Atta Graphics System
// rendererAPI.h
// Date: 2021-08-24
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_GRAPHICS_SYSTEM_RENDERER_API_H
#define ATTA_GRAPHICS_SYSTEM_RENDERER_API_H
#include <atta/core/stringId.h>
#include <atta/graphicsSystem/framebuffer.h>

namespace atta
{
    class RendererAPI
    {
    public:
        enum Type
        {
            OPENGL = 0,
            VULKAN
        };

        RendererAPI(Type type);
        virtual ~RendererAPI() = default;

        virtual void beginFrame() = 0;
        virtual void endFrame() = 0;

        virtual void renderMesh(StringId meshSid) = 0;
        virtual void renderQuad() = 0;
        virtual void renderQuad3() = 0;
        virtual void renderCube() = 0;
        virtual void framebufferToScreen(std::shared_ptr<Framebuffer> framebuffer) = 0;

        virtual void* getImGuiImage(StringId sid) const = 0;

        Type getType() const { return _type; }

    protected:
        Type _type;
    };
}

#endif// ATTA_GRAPHICS_SYSTEM_RENDERER_API_H
