//--------------------------------------------------
// Atta Graphics Module
// equiToCubemap.cpp
// Date: 2025-04-06
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/graphics/compute/equiToCubemap.h>
#include <atta/graphics/interface.h>
#include <atta/resource/interface.h>

namespace atta::graphics {

EquiToCubemap::EquiToCubemap() : _width(512), _height(512) {}

std::shared_ptr<gfx::Image> EquiToCubemap::createCubemap(StringId imageSid) {
    LOG_VERBOSE("gfx::EquiToCubemap", "Create cubemap from image [w]$0[]", imageSid);

    // Get equirectangular image
    std::shared_ptr<gfx::Image> image = gfx::Manager::getInstance().getImage(imageSid);
    if (image == nullptr) {
        LOG_ERROR("gfx::EquiToCubemap", "Could not create cubemap, image [w]$0[] does not exist", imageSid);
        return nullptr;
    }

    // Create cubemap image
    gfx::Image::CreateInfo imageInfo{};
    imageInfo.format = image->getFormat();
    imageInfo.width = _width;
    imageInfo.height = _height;
    imageInfo.isCubemap = true;
    imageInfo.debugName = imageSid.getString() + " Cubemap";
    std::shared_ptr<gfx::Image> cubemapImage = graphics::create<gfx::Image>(imageInfo);

    // Create Render Queue
    _renderQueue = graphics::create<RenderQueue>();

    // Create Framebuffer
    Framebuffer::Attachment attachment{};
    attachment.image = cubemapImage;
    Framebuffer::CreateInfo framebufferInfo{};
    framebufferInfo.attachments.push_back(attachment);
    framebufferInfo.width = _width;
    framebufferInfo.height = _height;
    framebufferInfo.clearColor = {0, 0, 0, 1};
    framebufferInfo.debugName = StringId("EquiToCubemap Framebuffer");
    std::shared_ptr<Framebuffer> framebuffer = graphics::create<Framebuffer>(framebufferInfo);

    // Create Render Pass
    RenderPass::CreateInfo renderPassInfo{};
    renderPassInfo.framebuffer = framebuffer;
    renderPassInfo.debugName = StringId("EquiToCubemap Render Pass");
    _renderPass = graphics::create<RenderPass>(renderPassInfo);

    //---------- Graphics pipeline ----------//
    // Create Pipeline
    Pipeline::CreateInfo pipelineInfo{};
    pipelineInfo.shader = graphics::create<Shader>("shaders/compute/equiToCubemap.asl");
    pipelineInfo.renderPass = _renderPass;
    renderPassInfo.debugName = StringId("EquiToCubemap Pipeline");
    _pipeline = graphics::create<Pipeline>(pipelineInfo);

    // Create Image Group
    _pipeline->createImageGroup(Pipeline::ImageGroupType::PER_FRAME, "equiImg");
    _pipeline->updateImageGroup("equiImg", {{"uEquirectangularMap", image}});

    // Define projection and view matrices
    mat4 proj = perspective(M_PI, 1.0f, 0.1f, 10.0f);
    std::array<mat4, 6> views = {
        // Positive X face: look toward +X, with Z as up.
        lookAt(vec3(0.0f), vec3(1.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 1.0f)),
        // Negative X face: look toward -X, with Z as up.
        lookAt(vec3(0.0f), vec3(-1.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 1.0f)),
        // Positive Y face: look toward +Y, with Z as up.
        lookAt(vec3(0.0f), vec3(0.0f, 1.0f, 0.0f), vec3(0.0f, 0.0f, 1.0f)),
        // Negative Y face: look toward -Y, with Z as up.
        lookAt(vec3(0.0f), vec3(0.0f, -1.0f, 0.0f), vec3(0.0f, 0.0f, 1.0f)),
        // Positive Z face: since forward (0,0,1) is parallel to global up, choose a different up (e.g. (0,1,0))
        lookAt(vec3(0.0f), vec3(0.0f, 0.0f, 1.0f), vec3(0.0f, 1.0f, 0.0f)),
        // Negative Z face: similarly choose (0,1,0) for up.
        lookAt(vec3(0.0f), vec3(0.0f, 0.0f, -1.0f), vec3(0.0f, 1.0f, 0.0f)),
    };

    // Render
    for (size_t i = 0; i < 6; i++) {
        framebuffer->setLayer(i);
        _renderQueue->begin();
        {
            _renderPass->begin(_renderQueue);
            {
                _pipeline->begin();
                {
                    _pipeline->setMat4("uProjection", proj);
                    _pipeline->setMat4("uView", views[i]);
                    _pipeline->setImageGroup("equiImg");
                    _pipeline->renderQuad();
                }
                _pipeline->end();
            }
            _renderPass->end();
        }
        _renderQueue->end();
    }

    return cubemapImage;
}

} // namespace atta::graphics
