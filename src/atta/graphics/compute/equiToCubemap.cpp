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
    // Vertex input layout
    Pipeline::CreateInfo pipelineInfo{};
    pipelineInfo.shader = graphics::create<Shader>("shaders/compute/equiToCubemap.asl");
    pipelineInfo.renderPass = _renderPass;
    renderPassInfo.debugName = StringId("EquiToCubemap Pipeline");
    _pipeline = graphics::create<Pipeline>(pipelineInfo);

    // Render
    for (size_t i = 0; i < 6; i++) {
        framebuffer->setLayer(i);
        _renderQueue->begin();
        {
            _renderPass->begin(_renderQueue);
            {
                _pipeline->begin();
                {
                    // TODO
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
