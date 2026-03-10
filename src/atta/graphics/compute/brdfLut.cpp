// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2020-2026 Breno Cunha Queiroz
#include <atta/graphics/compute/brdfLut.h>
#include <atta/graphics/interface.h>

namespace atta::graphics {

BrdfLut::BrdfLut() : _width(512), _height(512) {}

std::shared_ptr<gfx::Image> BrdfLut::generate() {
    LOG_VERBOSE("gfx::BrdfLut", "Generating BRDF LUT");

    // Create output image (RG16F: R=scale, G=bias)
    gfx::Image::CreateInfo imageInfo{};
    imageInfo.format = gfx::Image::Format::RG16F;
    imageInfo.width = _width;
    imageInfo.height = _height;
    imageInfo.isCubemap = false;
    imageInfo.debugName = "BRDF LUT";
    std::shared_ptr<gfx::Image> lutImage = graphics::create<gfx::Image>(imageInfo);

    // Create Render Queue
    _renderQueue = graphics::create<RenderQueue>();

    // Create Framebuffer
    Framebuffer::Attachment attachment{};
    attachment.image = lutImage;
    Framebuffer::CreateInfo framebufferInfo{};
    framebufferInfo.attachments.push_back(attachment);
    framebufferInfo.width = _width;
    framebufferInfo.height = _height;
    framebufferInfo.clearColor = {0, 0, 0, 0};
    framebufferInfo.debugName = StringId("BrdfLut Framebuffer");
    std::shared_ptr<Framebuffer> framebuffer = graphics::create<Framebuffer>(framebufferInfo);

    // Create Render Pass
    RenderPass::CreateInfo renderPassInfo{};
    renderPassInfo.framebuffer = framebuffer;
    renderPassInfo.debugName = StringId("BrdfLut Render Pass");
    _renderPass = graphics::create<RenderPass>(renderPassInfo);

    // Create Pipeline
    Pipeline::CreateInfo pipelineInfo{};
    pipelineInfo.shader = graphics::create<Shader>("shaders/compute/brdf.asl");
    pipelineInfo.renderPass = _renderPass;
    _pipeline = graphics::create<Pipeline>(pipelineInfo);

    // Render a full-screen quad — UV (0,0)-(1,1) maps to (NdotV, roughness)
    _renderQueue->begin();
    {
        _renderPass->begin(_renderQueue);
        {
            _pipeline->begin();
            {
                _pipeline->renderQuad();
            }
            _pipeline->end();
        }
        _renderPass->end();
    }
    _renderQueue->end();

    lutImage->prepareForSampling();

    return lutImage;
}

} // namespace atta::graphics
