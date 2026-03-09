// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2020-2026 Breno Cunha Queiroz
#include <atta/graphics/compute/irradiance.h>
#include <atta/graphics/interface.h>

namespace atta::graphics {

Irradiance::Irradiance() : _width(128), _height(128) {}

std::shared_ptr<gfx::Image> Irradiance::createIrradianceCubemap(StringId environmentCubemapSid) {
    LOG_VERBOSE("gfx::Irradiance", "Create irradiance cubemap from [w]$0[]", environmentCubemapSid);

    std::shared_ptr<gfx::Image> environmentCubemap = gfx::Manager::getInstance().getImage(environmentCubemapSid);
    if (environmentCubemap == nullptr) {
        LOG_ERROR("gfx::Irradiance", "Could not create irradiance cubemap, image [w]$0[] does not exist", environmentCubemapSid);
        return nullptr;
    }

    // Create irradiance cubemap image
    gfx::Image::CreateInfo imageInfo{};
    imageInfo.format = environmentCubemap->getFormat();
    imageInfo.width = _width;
    imageInfo.height = _height;
    imageInfo.isCubemap = true;
    imageInfo.debugName = environmentCubemapSid.getString() + " Irradiance";
    std::shared_ptr<gfx::Image> irradianceImage = graphics::create<gfx::Image>(imageInfo);

    // Create Render Queue
    _renderQueue = graphics::create<RenderQueue>();

    // Create Framebuffer
    Framebuffer::Attachment attachment{};
    attachment.image = irradianceImage;
    Framebuffer::CreateInfo framebufferInfo{};
    framebufferInfo.attachments.push_back(attachment);
    framebufferInfo.width = _width;
    framebufferInfo.height = _height;
    framebufferInfo.clearColor = {255, 0, 255, 255};
    framebufferInfo.debugName = StringId("Irradiance Framebuffer");
    std::shared_ptr<Framebuffer> framebuffer = graphics::create<Framebuffer>(framebufferInfo);

    // Create Render Pass
    RenderPass::CreateInfo renderPassInfo{};
    renderPassInfo.framebuffer = framebuffer;
    renderPassInfo.debugName = StringId("Irradiance Render Pass");
    _renderPass = graphics::create<RenderPass>(renderPassInfo);

    // Create Pipeline
    Pipeline::CreateInfo pipelineInfo{};
    pipelineInfo.shader = graphics::create<Shader>("shaders/compute/irradiance.asl");
    pipelineInfo.renderPass = _renderPass;
    _pipeline = graphics::create<Pipeline>(pipelineInfo);

    // Bind environment cubemap
    _pipeline->createImageGroup(Pipeline::ImageGroupType::PER_FRAME, "envImg");
    _pipeline->updateImageGroup("envImg", {{"uEnvironmentMap", environmentCubemap}});

    // Same face orientations as equiToCubemap (learnopengl convention, no Y-flip)
    mat4 proj = perspective(M_PI / 2.0f, 1.0f, 0.1f, 10.0f);
    std::array<mat4, 6> views = {
        lookAt(vec3(0.0f), vec3( 1.0f,  0.0f,  0.0f), vec3(0.0f, -1.0f,  0.0f)), // +X face
        lookAt(vec3(0.0f), vec3(-1.0f,  0.0f,  0.0f), vec3(0.0f, -1.0f,  0.0f)), // -X face
        lookAt(vec3(0.0f), vec3( 0.0f,  1.0f,  0.0f), vec3(0.0f,  0.0f,  1.0f)), // +Y face
        lookAt(vec3(0.0f), vec3( 0.0f, -1.0f,  0.0f), vec3(0.0f,  0.0f, -1.0f)), // -Y face
        lookAt(vec3(0.0f), vec3( 0.0f,  0.0f,  1.0f), vec3(0.0f, -1.0f,  0.0f)), // +Z face
        lookAt(vec3(0.0f), vec3( 0.0f,  0.0f, -1.0f), vec3(0.0f, -1.0f,  0.0f)), // -Z face
    };

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
                    _pipeline->setImageGroup("envImg");
                    _pipeline->renderCube();
                }
                _pipeline->end();
            }
            _renderPass->end();
        }
        _renderQueue->end();
    }

    irradianceImage->prepareForSampling();

    return irradianceImage;
}

} // namespace atta::graphics
