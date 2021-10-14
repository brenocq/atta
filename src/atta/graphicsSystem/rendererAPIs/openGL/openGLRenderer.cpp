//--------------------------------------------------
// Atta Graphics System
// rendererOpenGL.h
// Date: 2021-08-30
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/graphicsSystem/rendererAPIs/openGL/openGLRenderer.h>
#include <atta/graphicsSystem/rendererAPIs/openGL/openGLFramebuffer.h>
#include <atta/graphicsSystem/rendererAPIs/openGL/openGLImage.h>
#include <atta/graphicsSystem/rendererAPIs/openGL/base.h>
#include <atta/graphicsSystem/graphicsManager.h>
#include <atta/eventSystem/eventManager.h>
#include <atta/eventSystem/events/meshLoadEvent.h>
#include <atta/eventSystem/events/textureLoadEvent.h>
#include <atta/resourceSystem/resourceManager.h>
#include <atta/resourceSystem/resources/mesh.h>
#include <GLFW/glfw3.h>

namespace atta
{
    OpenGLRenderer::OpenGLRenderer(std::shared_ptr<Window> window):
        RendererAPI(RendererAPI::OPENGL), _window(window)
    {
        // Initialize GLAD
        int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
        ASSERT(status, "Failed to initialize Glad!");

        // Print info
        LOG_INFO("OpenGLRenderer", "GPU Info:");
        LOG_INFO("OpenGLRenderer", "  - Vendor: $0", glGetString(GL_VENDOR));
        LOG_INFO("OpenGLRenderer", "  - Renderer: $0", glGetString(GL_RENDERER));
        LOG_INFO("OpenGLRenderer", "  - Version: $0", glGetString(GL_VERSION));

#ifdef ATTA_DEBUG_BUILD
        // Enable Debug
        glEnable(GL_DEBUG_OUTPUT);
        glDebugMessageCallback([](GLenum source, GLenum type, GLuint id, 
                    GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
                {
                    switch (severity)
                    {
                        case GL_DEBUG_SEVERITY_HIGH:
                            LOG_ERROR("OpenGL Debug", "$0", message);
                            ASSERT(false, "OpenGL error");
                            break;
                        case GL_DEBUG_SEVERITY_MEDIUM:
                            LOG_WARN("OpenGL Debug", "$0", message);
                            break;
                        case GL_DEBUG_SEVERITY_LOW:
                            LOG_INFO("OpenGL Debug", "$0", message);
                            break;
                        case GL_DEBUG_SEVERITY_NOTIFICATION:
                            //LOG_VERBOSE("OpenGL Debug", "$0", message);
                            break;
                    }
                }, nullptr);
#endif// ATTA_DEBUG_BUILD

        // Check OpenGL version
        int versionMajor;
        int versionMinor;
        glGetIntegerv(GL_MAJOR_VERSION, &versionMajor);
        glGetIntegerv(GL_MINOR_VERSION, &versionMinor);
        ASSERT(versionMajor > 4 || (versionMajor == 4 && versionMinor >= 5), "Atta requires OpenGL >= 4.5");

        glEnable(GL_DEPTH_TEST);

        // Subscribe to events
        EventManager::subscribe<MeshLoadEvent>(BIND_EVENT_FUNC(OpenGLRenderer::onMeshLoadEvent));
        EventManager::subscribe<TextureLoadEvent>(BIND_EVENT_FUNC(OpenGLRenderer::onTextureLoadEvent));

        // Quad shader
        ShaderGroup::CreateInfo shaderGroupInfo {};
        shaderGroupInfo.shaderPaths = {"shaders/quad/shader.vert", "shaders/quad/shader.frag"};
        shaderGroupInfo.debugName = StringId("OpenGLRenderer Quad Shader Group");
        _quadShader = std::make_shared<OpenGLShaderGroup>(shaderGroupInfo);

        float quadVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
            // positions   // texCoords
            -1.0f,  1.0f,  0.0f, 1.0f,
            -1.0f, -1.0f,  0.0f, 0.0f,
            1.0f, -1.0f,  1.0f, 0.0f,

            -1.0f,  1.0f,  0.0f, 1.0f,
            1.0f, -1.0f,  1.0f, 0.0f,
            1.0f,  1.0f,  1.0f, 1.0f
        };

        // screen quad VAO
        glGenVertexArrays(1, &quadVAO);
        glGenBuffers(1, &quadVBO);
        glBindVertexArray(quadVAO);
        glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

        // Initialize meshes already loaded (create vertex/indices)
        for(auto meshSid : ResourceManager::getResources<Mesh>())
            initializeMesh(meshSid);
        // Initialize textures already loaded
        for(auto texSid : ResourceManager::getResources<Texture>())
            initializeTexture(texSid);
    }

    OpenGLRenderer::~OpenGLRenderer()
    {
        _openGLMeshes.clear();
    }

    void OpenGLRenderer::beginFrame()
    {
        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);
    }

    void OpenGLRenderer::endFrame()
    {

    }

    void OpenGLRenderer::renderMesh(StringId meshSid)
    {
        if(_openGLMeshes.find(meshSid.getId()) == _openGLMeshes.end())
        {
            LOG_WARN("OpenGLRenderer", "Trying to render mesh that was never initialized []$0[]", meshSid);
            return;
        }

        _openGLMeshes.at(meshSid.getId())->draw();
    }

    void* OpenGLRenderer::getImGuiImage(StringId sid) const
    {
        if(_openGLImages.find(sid.getId()) == _openGLImages.end())
        {
            LOG_WARN("OpenGLRenderer", "Trying to get ImGui image that was never initialized []$0[]", sid);
            return nullptr;
        }
        return reinterpret_cast<void*>(_openGLImages.at(sid.getId())->getId());
    }

    void OpenGLRenderer::onMeshLoadEvent(Event& event)
    {
        MeshLoadEvent& e = reinterpret_cast<MeshLoadEvent&>(event);
        initializeMesh(e.sid);
    }

    void OpenGLRenderer::onTextureLoadEvent(Event& event)
    {
        TextureLoadEvent& e = reinterpret_cast<TextureLoadEvent&>(event);
        initializeTexture(e.sid);
    }

    void OpenGLRenderer::framebufferToScreen(std::shared_ptr<Framebuffer> framebuffer)
    {
        glViewport(200, 200, framebuffer->getWidth(), framebuffer->getHeight());

        std::shared_ptr<OpenGLFramebuffer> openGLFramebuffer = std::static_pointer_cast<OpenGLFramebuffer>(framebuffer);
        std::shared_ptr<OpenGLImage> openGLImage = std::static_pointer_cast<OpenGLImage>(openGLFramebuffer->getImage(0));

        _quadShader->bind();
        glBindVertexArray(quadVAO);
        glDisable(GL_DEPTH_TEST);
        glBindTexture(GL_TEXTURE_2D, openGLImage->getId());
        glDrawArrays(GL_TRIANGLES, 0, 6);
    }

    void OpenGLRenderer::initializeMesh(StringId sid)
    {
        _openGLMeshes[sid.getId()] = std::make_shared<OpenGLMesh>(sid);
        LOG_DEBUG("OpenGLRenderer", "Mesh loaded! [w]$0[]", sid);
    }

    void OpenGLRenderer::initializeTexture(StringId sid)
    {
        Texture* texture = ResourceManager::get<Texture>(sid.getString());

        Image::CreateInfo info {};
        info.width = texture->getWidth();
        info.height = texture->getHeight();
        info.data = texture->getData();
        if(texture->getChannels() == 4)
            info.format = Image::Format::RGBA;
        else if(texture->getChannels() == 3)
            if(texture->getFormat() == Texture::Format::RGB8)
                info.format = Image::Format::RGB;
            else
                info.format = Image::Format::RGB16F;
        else if(texture->getChannels() == 1)
            info.format = Image::Format::RED;

        info.debugName = sid;
        _openGLImages[sid.getId()] = std::make_shared<OpenGLImage>(info);

        LOG_DEBUG("OpenGLRenderer", "Texture loaded! [w]$0[] -> $1 ($2)", sid, _openGLImages[sid.getId()]->getId(), info.format == Image::Format::RGB16F);
    }
}
