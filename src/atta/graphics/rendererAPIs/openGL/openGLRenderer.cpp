//--------------------------------------------------
// Atta Graphics Module
// rendererOpenGL.h
// Date: 2021-08-30
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/graphics/rendererAPIs/openGL/openGLRenderer.h>

#include <GLFW/glfw3.h>

#include <atta/event/events/imageLoad.h>
#include <atta/event/events/imageUpdate.h>
#include <atta/event/events/meshLoad.h>
#include <atta/event/manager.h>

#include <atta/graphics/manager.h>
#include <atta/graphics/rendererAPIs/openGL/base.h>
#include <atta/graphics/rendererAPIs/openGL/openGLFramebuffer.h>
#include <atta/graphics/rendererAPIs/openGL/openGLImage.h>

#include <atta/resource/manager.h>
#include <atta/resource/resources/image.h>
#include <atta/resource/resources/mesh.h>

namespace atta::graphics {

OpenGLRenderer::OpenGLRenderer(std::shared_ptr<Window> window) : RendererAPI(RendererAPI::OPENGL), _window(window) {
    // Initialize GLAD
    int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    ASSERT(status, "Failed to initialize Glad!");

    // Print info
    // LOG_INFO("OpenGLRenderer", "GPU Info:");
    // LOG_INFO("OpenGLRenderer", "  - Vendor: $0", glGetString(GL_VENDOR));
    // LOG_INFO("OpenGLRenderer", "  - Renderer: $0", glGetString(GL_RENDERER));
    // LOG_INFO("OpenGLRenderer", "  - Version: $0", glGetString(GL_VERSION));

#if defined(ATTA_DEBUG_BUILD) && !defined(ATTA_OS_WEB)
    // Enable Debug
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(
        [](GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam) {
            switch (severity) {
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
                // LOG_VERBOSE("OpenGL Debug", "$0", message);
                break;
            }
        },
        nullptr);
#endif // ATTA_DEBUG_BUILD

    // Check OpenGL version
    int versionMajor;
    int versionMinor;
    glGetIntegerv(GL_MAJOR_VERSION, &versionMajor);
    glGetIntegerv(GL_MINOR_VERSION, &versionMinor);
    ASSERT(versionMajor > 3 || (versionMajor == 3 && versionMinor >= 0), "Atta requires OpenGL >= 3.0");

    glEnable(GL_DEPTH_TEST);

    // Subscribe to events
    event::Manager::subscribe<event::MeshLoad>(BIND_EVENT_FUNC(OpenGLRenderer::onMeshLoadEvent));
    event::Manager::subscribe<event::ImageLoad>(BIND_EVENT_FUNC(OpenGLRenderer::onImageLoadEvent));
    event::Manager::subscribe<event::ImageUpdate>(BIND_EVENT_FUNC(OpenGLRenderer::onImageUpdateEvent));

    // Quad shader
    ShaderGroup::CreateInfo shaderGroupInfo{};
    shaderGroupInfo.shaderPaths = {"shaders/quad/shader.vert", "shaders/quad/shader.frag"};
    shaderGroupInfo.debugName = StringId("OpenGLRenderer Quad Shader Group");
    _quadShader = std::make_shared<OpenGLShaderGroup>(shaderGroupInfo);

    //---------- Quad ----------//
    {
        float vertices[] = {// vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
                            // positions   // texCoords
                            -1.0f, 1.0f, 0.0f, 1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 1.0f, -1.0f, 1.0f, 0.0f,

                            -1.0f, 1.0f, 0.0f, 1.0f, 1.0f,  -1.0f, 1.0f, 0.0f, 1.0f, 1.0f,  1.0f, 1.0f};

        // screen quad VAO
        glGenVertexArrays(1, &_quadVAO);
        glGenBuffers(1, &_quadVBO);
        glBindVertexArray(_quadVAO);
        glBindBuffer(GL_ARRAY_BUFFER, _quadVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    }

    //---------- Quad3 ----------//
    {
        float vertices[] = {
            // positions          // normal            // texCoords
            -1.0f, 1.0f,  1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
            1.0f,  -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, -1.0f, 1.0f,  1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
            1.0f,  -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f,  1.0f,  1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
        };

        // screen quad VAO
        glGenVertexArrays(1, &_quad3VAO);
        glGenBuffers(1, &_quad3VBO);
        glBindVertexArray(_quad3VAO);
        glBindBuffer(GL_ARRAY_BUFFER, _quad3VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    }

    //---------- Cube ----------//
    {
        float vertices[] = {
            // back face
            -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
            1.0f, 1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,   // top-right
            1.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,  // bottom-right
            1.0f, 1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,   // top-right
            -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
            -1.0f, 1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,  // top-left
            // front face
            -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom-left
            1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,  // bottom-right
            1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,   // top-right
            1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,   // top-right
            -1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,  // top-left
            -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom-left
            // left face
            -1.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,   // top-right
            -1.0f, 1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f,  // top-left
            -1.0f, -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // bottom-left
            -1.0f, -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // bottom-left
            -1.0f, -1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,  // bottom-right
            -1.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,   // top-right
                                                                // right face
            1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,     // top-left
            1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,   // bottom-right
            1.0f, 1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,    // top-right
            1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,   // bottom-right
            1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,     // top-left
            1.0f, -1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,    // bottom-left
            // bottom face
            -1.0f, -1.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, // top-right
            1.0f, -1.0f, -1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f,  // top-left
            1.0f, -1.0f, 1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,   // bottom-left
            1.0f, -1.0f, 1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,   // bottom-left
            -1.0f, -1.0f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,  // bottom-right
            -1.0f, -1.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, // top-right
            // top face
            -1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, // top-left
            1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,   // bottom-right
            1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,  // top-right
            1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,   // bottom-right
            -1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, // top-left
            -1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f   // bottom-left
        };
        glGenVertexArrays(1, &_cubeVAO);
        glGenBuffers(1, &_cubeVBO);
        // fill buffer
        glBindBuffer(GL_ARRAY_BUFFER, _cubeVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        // link vertex attributes
        glBindVertexArray(_cubeVAO);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    // Initialize meshes already loaded (create vertex/indices)
    for (auto meshSid : resource::Manager::getResources<resource::Mesh>())
        initializeMesh(meshSid);
    // Initialize textures already loaded
    for (auto imgSid : resource::Manager::getResources<resource::Image>())
        initializeImage(imgSid);
}

OpenGLRenderer::~OpenGLRenderer() {
    _openGLMeshes.clear();

    // Delete cubemaps
    for (auto tex : _openGLCubemaps)
        glDeleteTextures(1, &tex.second);
}

void OpenGLRenderer::beginFrame() {
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
}

void OpenGLRenderer::endFrame() {}

void OpenGLRenderer::renderMesh(StringId meshSid) {
    if (_openGLMeshes.find(meshSid.getId()) == _openGLMeshes.end()) {
        LOG_WARN("OpenGLRenderer", "Trying to render mesh that was never initialized '[w]$0[]'", meshSid);
        return;
    }

    _openGLMeshes.at(meshSid.getId())->draw();
}

void OpenGLRenderer::renderQuad() {
    // Can be used to render quads only with position and texture
    glBindVertexArray(_quadVAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

void OpenGLRenderer::renderQuad3() {
    // Rendereres should use this function to render quads
    glBindVertexArray(_quad3VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void OpenGLRenderer::renderCube() {
    // Render 3D cube in NDC
    glBindVertexArray(_cubeVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
}

void* OpenGLRenderer::getImGuiImage(StringId sid) const {
    if (_openGLImages.find(sid.getId()) == _openGLImages.end()) {
        LOG_WARN("OpenGLRenderer", "Trying to get ImGui image that was never initialized '[w]$0[]'", sid);
        return nullptr;
    }
    return reinterpret_cast<void*>(_openGLImages.at(sid.getId())->getImGuiImage());
}

void OpenGLRenderer::onMeshLoadEvent(event::Event& event) {
    event::MeshLoad& e = reinterpret_cast<event::MeshLoad&>(event);
    initializeMesh(e.sid);
}

void OpenGLRenderer::onImageLoadEvent(event::Event& event) {
    event::ImageLoad& e = reinterpret_cast<event::ImageLoad&>(event);
    initializeImage(e.sid);
}

void OpenGLRenderer::onImageUpdateEvent(event::Event& event) {
    event::ImageUpdate& e = reinterpret_cast<event::ImageUpdate&>(event);
    resource::Image* image = resource::Manager::get<resource::Image>(e.sid.getString());
    if (image == nullptr) {
        LOG_WARN("OpenGLRenderer", "Could not initialize OpenGL texture from [w]$0[], image resource does not exists", e.sid.getString());
        return;
    }
    if (_openGLImages.find(e.sid.getId()) == _openGLImages.end()) {
        LOG_WARN("OpenGLRenderer", "OpenGL texture [w]$0[] was not loaded before update", e.sid.getString());
        return;
    }

    std::shared_ptr<OpenGLImage> openGLImage = _openGLImages[e.sid.getId()];
    if (openGLImage->getWidth() != image->getWidth() || openGLImage->getHeight() != image->getHeight())
        openGLImage->resize(image->getWidth(), image->getHeight());
    else
        openGLImage->write(image->getData());
}

void OpenGLRenderer::renderFramebufferToQuad(std::shared_ptr<Framebuffer> framebuffer) {
    glViewport(200, 200, framebuffer->getWidth(), framebuffer->getHeight());

    LOG_DEBUG("OpenGLRenderer", "Framebuffer from framebufferToScreen");
    std::shared_ptr<OpenGLFramebuffer> openGLFramebuffer = std::static_pointer_cast<OpenGLFramebuffer>(framebuffer);
    std::shared_ptr<OpenGLImage> openGLImage = std::static_pointer_cast<OpenGLImage>(openGLFramebuffer->getImage(0));

    _quadShader->bind();
    glDisable(GL_DEPTH_TEST);
    glBindTexture(GL_TEXTURE_2D, openGLImage->getId());
    renderQuad();
}

void OpenGLRenderer::generateCubemap(StringId textureSid, mat4 rotationMatrix) {
    // Ensure that the texture was loaded
    resource::Manager::get<resource::Image>(fs::path(textureSid.getString()));

    // Output texture id
    unsigned int cubemap;

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    unsigned int captureFBO;
    unsigned int captureRBO;
    glGenFramebuffers(1, &captureFBO);
    glGenRenderbuffers(1, &captureRBO);

    glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
    glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 512, 512);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, captureRBO);

    //---------- Equirectangular to cubemap ----------//
    ShaderGroup::CreateInfo shaderGroupInfo{};
    shaderGroupInfo.shaderPaths = {"shaders/compute/equiToCubemap.vert", "shaders/compute/equiToCubemap.frag"};
    shaderGroupInfo.debugName = StringId("EquiToCubemap Shader Group");
    std::shared_ptr<ShaderGroup> shader = Manager::create<ShaderGroup>(shaderGroupInfo);

    // Create cubemap texture
    glGenTextures(1, &cubemap);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap);
    for (unsigned int i = 0; i < 6; ++i) {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 512, 512, 0, GL_RGB, GL_FLOAT, nullptr);
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Projection and view matrices
    mat4 captureProjection = perspective(radians(90.0f), 1.0f, 0.1f, 10.0f);

    mat3 rot{rotationMatrix};
    mat4 captureViews[] = {
        // Converting from equirectangular to atta coordinate system
        lookAt(vec3(0.0f, 0.0f, 0.0f), rot * vec3(0.0f, 0.0f, 1.0f), rot * vec3(-1.0f, 0.0f, 0.0f)),  // X+
        lookAt(vec3(0.0f, 0.0f, 0.0f), rot * vec3(0.0f, 0.0f, -1.0f), rot * vec3(-1.0f, 0.0f, 0.0f)), // X-
        lookAt(vec3(0.0f, 0.0f, 0.0f), rot * vec3(1.0f, 0.0f, 0.0f), rot * vec3(0.0f, 1.0f, 0.0f)),   // Y+
        lookAt(vec3(0.0f, 0.0f, 0.0f), rot * vec3(-1.0f, 0.0f, 0.0f), rot * vec3(0.0f, -1.0f, 0.0f)), // Y-
        lookAt(vec3(0.0f, 0.0f, 0.0f), rot * vec3(0.0f, 1.0f, 0.0f), rot * vec3(-1.0f, 0.0f, 0.0f)),  // Z+
        lookAt(vec3(0.0f, 0.0f, 0.0f), rot * vec3(0.0f, -1.0f, 0.0f), rot * vec3(-1.0f, 0.0f, 0.0f))  // Z-
    };

    // Convert texture to cubemap
    shader->bind();
    shader->setMat4("projection", transpose(captureProjection));
    shader->setTexture("equirectangularMap", textureSid);
    glViewport(0, 0, 512, 512);
    glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
    for (unsigned int i = 0; i < 6; i++) {
        shader->setMat4("view", transpose(captureViews[i]));
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, cubemap, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        renderCube();
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // Delete old cubemap if necessary
    if (_openGLCubemaps.find(textureSid.getId()) != _openGLCubemaps.end())
        glDeleteTextures(1, &_openGLCubemaps[textureSid.getId()]);

    _openGLCubemaps[textureSid.getId()] = cubemap;
}

void OpenGLRenderer::generateProcessedCubemap(GenerateProcessedCubemapInfo gpcInfo) {
    unsigned int cubemap;
    ASSERT(gpcInfo.numMipLevels >= 1,
           "[OpenGLRenderer] [w](generateProcessedCubemap)[] The number of mipmap levels must be greater than 0, but it is [w]$0[]",
           gpcInfo.numMipLevels);
    ASSERT(gpcInfo.width > 0, "[OpenGLRenderer] [w](generateProcessedCubemap)[] The width should be grater than 0, but it is [w]$0[]", gpcInfo.width);
    ASSERT(gpcInfo.height > 0, "[OpenGLRenderer] [w](generateProcessedCubemap)[] The height should be grater than 0, but it is [w]$0[]",
           gpcInfo.height);
    ASSERT(gpcInfo.cubemapSid != "Not defined"_sid, "[OpenGLRenderer] [w](generateProcessedCubemap)[] cubemapSid was not defined");
    ASSERT(gpcInfo.func,
           "[OpenGLRenderer] [w](generateProcessedCubemap)[] func must be defined. The shader probably needs the view and projection matrices");

    glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

    unsigned int captureFBO;
    unsigned int captureRBO;
    glGenFramebuffers(1, &captureFBO);
    glGenRenderbuffers(1, &captureRBO);

    glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
    glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, captureRBO);

    // Projection and view matrices
    mat4 captureProjection = perspective(radians(90.0f), 1.0f, 0.1f, 10.0f);
    mat4 captureViews[] = {lookAt(vec3(0.0f, 0.0f, 0.0f), vec3(1.0f, 0.0f, 0.0f), vec3(0.0f, -1.0f, 0.0f)),
                           lookAt(vec3(0.0f, 0.0f, 0.0f), vec3(-1.0f, 0.0f, 0.0f), vec3(0.0f, -1.0f, 0.0f)),
                           lookAt(vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f), vec3(0.0f, 0.0f, 1.0f)),
                           lookAt(vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, -1.0f, 0.0f), vec3(0.0f, 0.0f, -1.0f)),
                           lookAt(vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 1.0f), vec3(0.0f, -1.0f, 0.0f)),
                           lookAt(vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, -1.0f), vec3(0.0f, -1.0f, 0.0f))};

    glGenTextures(1, &cubemap);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap);
    for (unsigned int i = 0; i < 6; i++)
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, gpcInfo.width, gpcInfo.height, 0, GL_RGB, GL_FLOAT, nullptr);

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, gpcInfo.numMipLevels == 1 ? GL_LINEAR : GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    if (gpcInfo.numMipLevels > 1)
        glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

    gpcInfo.shader->bind();
    glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
    for (unsigned int mip = 0; mip < gpcInfo.numMipLevels; mip++) {
        // reisze framebuffer according to mip-level size.
        unsigned int mipWidth = gpcInfo.width * std::pow(0.5, mip);
        unsigned int mipHeight = gpcInfo.height * std::pow(0.5, mip);
        glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, mipWidth, mipHeight);
        glViewport(0, 0, mipWidth, mipHeight);

        for (unsigned int i = 0; i < 6; i++) {
            if (gpcInfo.func)
                gpcInfo.func(gpcInfo.shader, captureProjection, captureViews[i], i, mip);

            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, cubemap, mip);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            Manager::getRendererAPI()->renderCube();
        }
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    _openGLCubemaps[gpcInfo.cubemapSid.getId()] = cubemap;
}

void OpenGLRenderer::generateProcessedTexture(GenerateProcessedTextureInfo gptInfo) {
    ASSERT(gptInfo.textureSid != "Not defined"_sid, "[OpenGLRenderer] [w](generateProcessedTexture)[] textureSid was not defined");

    std::shared_ptr<OpenGLImage> image;
    Image::CreateInfo info = gptInfo.imageInfo;
    image = std::make_shared<OpenGLImage>(info);

    unsigned int captureFBO;
    unsigned int captureRBO;
    glGenFramebuffers(1, &captureFBO);
    glGenRenderbuffers(1, &captureRBO);

    glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
    glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, captureRBO);

    // then re-configure capture framebuffer object and render screen-space quad with BRDF shader.
    glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
    glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, image->getWidth(), image->getHeight());
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, image->getId(), 0);

    glViewport(0, 0, image->getWidth(), image->getHeight());
    gptInfo.shader->bind();
    if (gptInfo.func)
        gptInfo.func(gptInfo.shader);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    Manager::getRendererAPI()->renderQuad3();

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    _openGLImages[gptInfo.textureSid.getId()] = image;
}

void OpenGLRenderer::initializeMesh(StringId sid) {
    _openGLMeshes[sid.getId()] = std::make_shared<OpenGLMesh>(sid);
    // LOG_DEBUG("OpenGLRenderer", "Mesh loaded! [w]$0[]", sid);
}

void OpenGLRenderer::initializeImage(StringId sid) {
    resource::Image* image = resource::Manager::get<resource::Image>(sid.getString());
    if (image == nullptr)
        LOG_WARN("OpenGLRenderer", "Could not initialize OpenGL image from [w]$0[]", sid.getString());

    Image::CreateInfo info{};
    info.width = image->getWidth();
    info.height = image->getHeight();
    info.data = image->getData();
    if (image->getChannels() == 4)
        info.format = Image::Format::RGBA;
    else if (image->getChannels() == 3)
        if (image->getFormat() == resource::Image::Format::RGB8)
            info.format = Image::Format::RGB;
        else
            info.format = Image::Format::RGB16F;
    else if (image->getChannels() == 1)
        info.format = Image::Format::RED;

    info.debugName = sid;
    _openGLImages[sid.getId()] = std::make_shared<OpenGLImage>(info);

    // LOG_DEBUG("OpenGLRenderer", "Texture loaded! [w]$0[] -> $1", sid, _openGLImages[sid.getId()]->getId());
}

} // namespace atta::graphics
