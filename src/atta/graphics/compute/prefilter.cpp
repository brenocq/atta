// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2020-2026 Breno Cunha Queiroz
#include <atta/graphics/compute/prefilter.h>
#include <atta/graphics/interface.h>

namespace atta::graphics {

Prefilter::Prefilter() {}

std::shared_ptr<gfx::Image> Prefilter::createPrefilterCubemap(StringId environmentCubemapSid) {
    LOG_VERBOSE("gfx::Prefilter", "Create prefilter cubemap from [w]$0[]", environmentCubemapSid);

    std::shared_ptr<gfx::Image> environmentCubemap = gfx::Manager::getInstance().getImage(environmentCubemapSid);
    if (environmentCubemap == nullptr) {
        LOG_ERROR("gfx::Prefilter", "Could not create prefilter cubemap, image [w]$0[] does not exist", environmentCubemapSid);
        return nullptr;
    }

    // Create prefilter cubemap image with mip levels
    gfx::Image::CreateInfo imageInfo{};
    imageInfo.format = environmentCubemap->getFormat();
    imageInfo.width = _baseSize;
    imageInfo.height = _baseSize;
    imageInfo.mipLevels = _numMips;
    imageInfo.isCubemap = true;
    imageInfo.debugName = environmentCubemapSid.getString() + " Prefilter";
    std::shared_ptr<gfx::Image> prefilterImage = graphics::create<gfx::Image>(imageInfo);

    // Create render queue (reused for all passes)
    auto renderQueue = graphics::create<RenderQueue>();

    // Same face orientations as equiToCubemap/irradiance (learnopengl convention, no Y-flip)
    mat4 proj = perspective(M_PI / 2.0f, 1.0f, 0.1f, 10.0f);
    std::array<mat4, 6> views = {
        lookAt(vec3(0.0f), vec3( 1.0f,  0.0f,  0.0f), vec3(0.0f, -1.0f,  0.0f)), // +X face
        lookAt(vec3(0.0f), vec3(-1.0f,  0.0f,  0.0f), vec3(0.0f, -1.0f,  0.0f)), // -X face
        lookAt(vec3(0.0f), vec3( 0.0f,  1.0f,  0.0f), vec3(0.0f,  0.0f,  1.0f)), // +Y face
        lookAt(vec3(0.0f), vec3( 0.0f, -1.0f,  0.0f), vec3(0.0f,  0.0f, -1.0f)), // -Y face
        lookAt(vec3(0.0f), vec3( 0.0f,  0.0f,  1.0f), vec3(0.0f, -1.0f,  0.0f)), // +Z face
        lookAt(vec3(0.0f), vec3( 0.0f,  0.0f, -1.0f), vec3(0.0f, -1.0f,  0.0f)), // -Z face
    };

    // One framebuffer + renderpass + pipeline per mip level (different dimensions)
    for (uint32_t mip = 0; mip < _numMips; mip++) {
        uint32_t mipSize = std::max(1u, _baseSize >> mip);
        float roughness = static_cast<float>(mip) / static_cast<float>(_numMips - 1);

        // Create framebuffer at this mip's resolution
        Framebuffer::Attachment attachment{};
        attachment.image = prefilterImage;
        Framebuffer::CreateInfo framebufferInfo{};
        framebufferInfo.attachments.push_back(attachment);
        framebufferInfo.width = mipSize;
        framebufferInfo.height = mipSize;
        framebufferInfo.clearColor = {0, 0, 0, 0};
        framebufferInfo.debugName = StringId("Prefilter Framebuffer mip" + std::to_string(mip));
        auto framebuffer = graphics::create<Framebuffer>(framebufferInfo);

        // Create render pass
        RenderPass::CreateInfo renderPassInfo{};
        renderPassInfo.framebuffer = framebuffer;
        renderPassInfo.debugName = StringId("Prefilter Render Pass mip" + std::to_string(mip));
        auto renderPass = graphics::create<RenderPass>(renderPassInfo);

        // Create pipeline (reuses same shader, different renderpass/framebuffer per mip)
        Pipeline::CreateInfo pipelineInfo{};
        pipelineInfo.shader = graphics::create<Shader>("shaders/compute/prefilter.asl");
        pipelineInfo.renderPass = renderPass;
        auto pipeline = graphics::create<Pipeline>(pipelineInfo);

        // Bind environment cubemap
        pipeline->createImageGroup(Pipeline::ImageGroupType::PER_FRAME, "envImg");
        pipeline->updateImageGroup("envImg", {{"uEnvironmentMap", environmentCubemap}});

        // Render 6 faces at this mip level
        for (uint32_t face = 0; face < 6; face++) {
            framebuffer->setLayerAndMip(face, mip);
            renderQueue->begin();
            {
                renderPass->begin(renderQueue);
                {
                    pipeline->begin();
                    {
                        pipeline->setMat4("uProjection", proj);
                        pipeline->setMat4("uView", views[face]);
                        pipeline->setFloat("uRoughness", roughness);
                        pipeline->setImageGroup("envImg");
                        pipeline->renderCube();
                    }
                    pipeline->end();
                }
                renderPass->end();
            }
            renderQueue->end();
        }
    }

    prefilterImage->prepareForSampling();

    return prefilterImage;
}

} // namespace atta::graphics
