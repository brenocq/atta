//--------------------------------------------------
// Atta Graphics Module
// openGLAPI.h
// Date: 2021-08-30
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/graphics/apis/openGL/openGLAPI.h>

#include <GLFW/glfw3.h>

#include <atta/graphics/apis/openGL/base.h>
#include <atta/graphics/apis/openGL/framebuffer.h>
#include <atta/graphics/apis/openGL/image.h>
#include <atta/graphics/interface.h>

#include <atta/resource/interface.h>
#include <atta/resource/resources/image.h>
#include <atta/resource/resources/mesh.h>

namespace atta::graphics {

OpenGLAPI::OpenGLAPI(std::shared_ptr<Window> window) : GraphicsAPI(GraphicsAPI::OPENGL, window) {}

OpenGLAPI::~OpenGLAPI() {}

void OpenGLAPI::startUp() {
    // Initialize GLAD
    int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    ASSERT(status, "Failed to initialize Glad!");

    // Print info
    // LOG_INFO("graphics::OpenGLAPI", "GPU Info:");
    // LOG_INFO("graphics::OpenGLAPI", "  - Vendor: $0", glGetString(GL_VENDOR));
    // LOG_INFO("graphics::OpenGLAPI", "  - Renderer: $0", glGetString(GL_RENDERER));
    // LOG_INFO("graphics::OpenGLAPI", "  - Version: $0", glGetString(GL_VERSION));

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
    glEnable(GL_PROGRAM_POINT_SIZE);
}

void OpenGLAPI::shutDown() {}

void OpenGLAPI::waitDevice() {
    glFlush();
    glFinish();
}

void OpenGLAPI::beginFrame() {}

void OpenGLAPI::endFrame() { _window->swapBuffers(); }

void* OpenGLAPI::getImGuiImage(StringId sid) const {
    const auto& images = gfx::Manager::getInstance().getImages();
    if (images.find(sid.getId()) == images.end()) {
        LOG_WARN("gfx::OpenGLAPI", "Trying to get ImGui image that was never initialized '[w]$0[]'", sid);
        return nullptr;
    }
    return reinterpret_cast<void*>(images.at(sid.getId())->getImGuiImage());
}

void OpenGLAPI::renderFramebufferToQuad(std::shared_ptr<Framebuffer> framebuffer) {
    // glViewport(200, 200, framebuffer->getWidth(), framebuffer->getHeight());

    // LOG_DEBUG("gfx::OpenGLAPI", "Framebuffer from framebufferToScreen");
    // std::shared_ptr<gl::Framebuffer> openGLFramebuffer = std::static_pointer_cast<gl::Framebuffer>(framebuffer);
    // std::shared_ptr<gl::Image> openGLImage = std::static_pointer_cast<gl::Image>(openGLFramebuffer->getImage(0));

    //_quadShader->bind();
    // glDisable(GL_DEPTH_TEST);
    // glBindTexture(GL_TEXTURE_2D, openGLImage->getHandle());
    // renderQuad();
}

void OpenGLAPI::generateCubemap(StringId textureSid, mat4 rotationMatrix) {
    //// Ensure that the texture was loaded
    // resource::get<resource::Image>(fs::path(textureSid.getString()));

    //// Output texture id
    // unsigned int cubemap;

    // glEnable(GL_DEPTH_TEST);
    // glDepthFunc(GL_LEQUAL);

    // unsigned int captureFBO;
    // unsigned int captureRBO;
    // glGenFramebuffers(1, &captureFBO);
    // glGenRenderbuffers(1, &captureRBO);

    // glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
    // glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
    // glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 512, 512);
    // glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, captureRBO);

    ////---------- Equirectangular to cubemap ----------//
    // std::shared_ptr<Shader> shader = graphics::create<Shader>("shaders/compute/equiToCubemap.asl");

    //// Create cubemap texture
    // glGenTextures(1, &cubemap);
    // glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap);
    // for (unsigned int i = 0; i < 6; ++i) {
    //     glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 512, 512, 0, GL_RGB, GL_FLOAT, nullptr);
    // }
    // glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    // glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    // glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    // glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    // glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    //// Projection and view matrices
    // mat4 captureProjection = perspective(radians(90.0f), 1.0f, 0.1f, 10.0f);

    // mat3 rot{rotationMatrix};
    // mat4 captureViews[] = {
    //     // Converting from equirectangular to atta coordinate system
    //     lookAt(vec3(0.0f, 0.0f, 0.0f), rot * vec3(0.0f, 0.0f, 1.0f), rot * vec3(-1.0f, 0.0f, 0.0f)),  // X+
    //     lookAt(vec3(0.0f, 0.0f, 0.0f), rot * vec3(0.0f, 0.0f, -1.0f), rot * vec3(-1.0f, 0.0f, 0.0f)), // X-
    //     lookAt(vec3(0.0f, 0.0f, 0.0f), rot * vec3(1.0f, 0.0f, 0.0f), rot * vec3(0.0f, 1.0f, 0.0f)),   // Y+
    //     lookAt(vec3(0.0f, 0.0f, 0.0f), rot * vec3(-1.0f, 0.0f, 0.0f), rot * vec3(0.0f, -1.0f, 0.0f)), // Y-
    //     lookAt(vec3(0.0f, 0.0f, 0.0f), rot * vec3(0.0f, 1.0f, 0.0f), rot * vec3(-1.0f, 0.0f, 0.0f)),  // Z+
    //     lookAt(vec3(0.0f, 0.0f, 0.0f), rot * vec3(0.0f, -1.0f, 0.0f), rot * vec3(-1.0f, 0.0f, 0.0f))  // Z-
    // };

    //// Convert texture to cubemap
    // shader->bind();
    // shader->setMat4("projection", transpose(captureProjection));
    // shader->setImage("equirectangularMap", textureSid);
    // glViewport(0, 0, 512, 512);
    // glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
    // for (unsigned int i = 0; i < 6; i++) {
    //     shader->setMat4("view", transpose(captureViews[i]));
    //     glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, cubemap, 0);
    //     glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //     renderCube();
    // }
    // glBindFramebuffer(GL_FRAMEBUFFER, 0);

    //// Delete old cubemap if necessary
    // if (_openGLCubemaps.find(textureSid.getId()) != _openGLCubemaps.end())
    //     glDeleteTextures(1, &_openGLCubemaps[textureSid.getId()]);

    //_openGLCubemaps[textureSid.getId()] = cubemap;
}

void OpenGLAPI::generateProcessedCubemap(GenerateProcessedCubemapInfo gpcInfo) {
    // unsigned int cubemap;
    // ASSERT(gpcInfo.numMipLevels >= 1,
    //        "[OpenGLAPI] [w](generateProcessedCubemap)[] The number of mipmap levels must be greater than 0, but it is [w]$0[]",
    //        gpcInfo.numMipLevels);
    // ASSERT(gpcInfo.width > 0, "[OpenGLAPI] [w](generateProcessedCubemap)[] The width should be grater than 0, but it is [w]$0[]", gpcInfo.width);
    // ASSERT(gpcInfo.height > 0, "[OpenGLAPI] [w](generateProcessedCubemap)[] The height should be grater than 0, but it is [w]$0[]",
    // gpcInfo.height); ASSERT(gpcInfo.cubemapSid != "Not defined"_sid, "[OpenGLAPI] [w](generateProcessedCubemap)[] cubemapSid was not defined");
    // ASSERT(gpcInfo.func,
    //        "[OpenGLAPI] [w](generateProcessedCubemap)[] func must be defined. The shader probably needs the view and projection matrices");

    // glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

    // unsigned int captureFBO;
    // unsigned int captureRBO;
    // glGenFramebuffers(1, &captureFBO);
    // glGenRenderbuffers(1, &captureRBO);

    // glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
    // glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
    // glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, captureRBO);

    //// Projection and view matrices
    // mat4 captureProjection = perspective(radians(90.0f), 1.0f, 0.1f, 10.0f);
    // mat4 captureViews[] = {lookAt(vec3(0.0f, 0.0f, 0.0f), vec3(1.0f, 0.0f, 0.0f), vec3(0.0f, -1.0f, 0.0f)),
    //                        lookAt(vec3(0.0f, 0.0f, 0.0f), vec3(-1.0f, 0.0f, 0.0f), vec3(0.0f, -1.0f, 0.0f)),
    //                        lookAt(vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f), vec3(0.0f, 0.0f, 1.0f)),
    //                        lookAt(vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, -1.0f, 0.0f), vec3(0.0f, 0.0f, -1.0f)),
    //                        lookAt(vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 1.0f), vec3(0.0f, -1.0f, 0.0f)),
    //                        lookAt(vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, -1.0f), vec3(0.0f, -1.0f, 0.0f))};

    // glGenTextures(1, &cubemap);
    // glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap);
    // for (unsigned int i = 0; i < 6; i++)
    //     glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, gpcInfo.width, gpcInfo.height, 0, GL_RGB, GL_FLOAT, nullptr);

    // glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    // glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    // glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    // glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, gpcInfo.numMipLevels == 1 ? GL_LINEAR : GL_LINEAR_MIPMAP_LINEAR);
    // glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // if (gpcInfo.numMipLevels > 1)
    //     glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

    // gpcInfo.shader->bind();
    // glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
    // for (unsigned int mip = 0; mip < gpcInfo.numMipLevels; mip++) {
    //     // reisze framebuffer according to mip-level size.
    //     unsigned int mipWidth = gpcInfo.width * std::pow(0.5, mip);
    //     unsigned int mipHeight = gpcInfo.height * std::pow(0.5, mip);
    //     glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
    //     glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, mipWidth, mipHeight);
    //     glViewport(0, 0, mipWidth, mipHeight);

    //    for (unsigned int i = 0; i < 6; i++) {
    //        if (gpcInfo.func)
    //            gpcInfo.func(gpcInfo.shader, captureProjection, captureViews[i], i, mip);

    //        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, cubemap, mip);
    //        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //        graphics::getGraphicsAPI()->renderCube();
    //    }
    //}
    // glBindFramebuffer(GL_FRAMEBUFFER, 0);

    //_openGLCubemaps[gpcInfo.cubemapSid.getId()] = cubemap;
}

void OpenGLAPI::generateProcessedTexture(GenerateProcessedTextureInfo gptInfo) {
    // ASSERT(gptInfo.textureSid != "Not defined"_sid, "[OpenGLAPI] [w](generateProcessedTexture)[] textureSid was not defined");

    // std::shared_ptr<gl::Image> image;
    // Image::CreateInfo info = gptInfo.imageInfo;
    // image = std::make_shared<gl::Image>(info);

    // unsigned int captureFBO;
    // unsigned int captureRBO;
    // glGenFramebuffers(1, &captureFBO);
    // glGenRenderbuffers(1, &captureRBO);

    // glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
    // glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
    // glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, captureRBO);

    //// then re-configure capture framebuffer object and render screen-space quad with BRDF shader.
    // glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
    // glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
    // glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, image->getWidth(), image->getHeight());
    // glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, image->getHandle(), 0);

    // glViewport(0, 0, image->getWidth(), image->getHeight());
    // gptInfo.shader->bind();
    // if (gptInfo.func)
    //     gptInfo.func(gptInfo.shader);

    // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // graphics::getGraphicsAPI()->renderQuad3();

    // glBindFramebuffer(GL_FRAMEBUFFER, 0);
    //_openGLImages[gptInfo.textureSid.getId()] = image;
}

} // namespace atta::graphics
