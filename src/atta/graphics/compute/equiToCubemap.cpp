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

EquiToCubemap::EquiToCubemap() : _width(512), _height(512) {
    // Render Queue
    _renderQueue = graphics::create<RenderQueue>();

    // Framebuffer
    Framebuffer::CreateInfo framebufferInfo{};
    framebufferInfo.attachments.push_back({Image::Format::RGBA});
    framebufferInfo.width = _width;
    framebufferInfo.height = _height;
    framebufferInfo.clearColor = {1, 0, 0, 1};
    framebufferInfo.debugName = StringId("EquiToCubemap Framebuffer");
    std::shared_ptr<Framebuffer> framebuffer = graphics::create<Framebuffer>(framebufferInfo);

    // Render Pass
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
}

std::shared_ptr<gfx::Image> EquiToCubemap::createCubemap(StringId imageSid) {
    LOG_VERBOSE("gfx::EquiToCubemap", "Create cubemap from image [w]$0[]", imageSid);

    std::shared_ptr<gfx::Image> image = gfx::Manager::getInstance().getImage(imageSid);
    if (image == nullptr) {
        LOG_ERROR("gfx::EquiToCubemap", "Could not create cubemap, image [w]$0[] does not exist", imageSid);
        return nullptr;
    }

    gfx::Image::CreateInfo imageInfo{};
    imageInfo.format = Image::Format::RGBA;
    imageInfo.width = _width;
    imageInfo.height = _height;
    imageInfo.isCubemap = true;
    imageInfo.debugName = imageSid.getString() + " Cubemap";
    std::shared_ptr<gfx::Image> cubemapImage = graphics::create<gfx::Image>(imageInfo);

    // Render
    _renderQueue->begin();
    {
        for (size_t i = 0; i < 6; i++) {
            _renderPass->getFramebuffer()->setLayer(i);
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
    }
    _renderQueue->end();

    return cubemapImage;
}

} // namespace atta::graphics
