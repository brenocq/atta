//--------------------------------------------------
// Atta Graphics System
// openGLRenderer.h
// Date: 2021-08-30
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_GRAPHICS_SYSTEM_RENDERER_APIS_OPENGL_OPENGL_RENDERER_H
#define ATTA_GRAPHICS_SYSTEM_RENDERER_APIS_OPENGL_OPENGL_RENDERER_H
#include <atta/graphicsSystem/rendererAPI.h>
#include <atta/graphicsSystem/rendererAPIs/openGL/base.h>
#include <atta/graphicsSystem/windows/window.h>
#include <atta/graphicsSystem/rendererAPIs/openGL/openGLMesh.h>
#include <atta/graphicsSystem/rendererAPIs/openGL/openGLImage.h>
#include <atta/graphicsSystem/rendererAPIs/openGL/openGLShaderGroup.h>

namespace atta
{
    class OpenGLRenderer final : public RendererAPI
    {
    public:
        OpenGLRenderer(std::shared_ptr<Window> window);
        ~OpenGLRenderer();

        void beginFrame() override;
        void endFrame() override;

        void renderMesh(StringId meshSid) override;
        void renderQuad() override;
        void renderCube() override;
        void framebufferToScreen(std::shared_ptr<Framebuffer> framebuffer) override;

        void* getImGuiImage(StringId sid) const override;

        // Handle events
        void onMeshLoadEvent(Event& event);
        void onTextureLoadEvent(Event& event);

        std::unordered_map<StringHash, std::shared_ptr<OpenGLImage>> getOpenGLImages() const { return _openGLImages; };

    private:
        void initializeMesh(StringId sid);
        void initializeTexture(StringId sid);

        std::shared_ptr<Window> _window;
        std::unordered_map<StringHash, std::shared_ptr<OpenGLMesh>> _openGLMeshes;
        std::unordered_map<StringHash, std::shared_ptr<OpenGLImage>> _openGLImages;

        std::shared_ptr<OpenGLShaderGroup> _quadShader;

        OpenGLId _quadVBO;
        OpenGLId _quadVAO;
        OpenGLId _cubeVBO;
        OpenGLId _cubeVAO;
    };
}

#endif// ATTA_GRAPHICS_SYSTEM_RENDERER_APIS_OPENGL_OPENGL_RENDERER_H
