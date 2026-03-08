// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2020-2026 Breno Cunha Queiroz
#pragma once

#include <atta/graphics/apis/graphicsAPI.h>
#include <atta/graphics/apis/openGL/image.h>
#include <atta/graphics/apis/openGL/mesh.h>
#include <atta/graphics/apis/openGL/shader.h>
#include <atta/graphics/windows/window.h>

namespace atta::graphics {

class OpenGLAPI final : public GraphicsAPI {
  public:
    OpenGLAPI(std::shared_ptr<Window> window);
    ~OpenGLAPI();

    void startUp() override;
    void shutDown() override;
    void waitDevice() override;

    void beginFrame() override;
    void endFrame() override;

    void renderFramebufferToQuad(std::shared_ptr<Framebuffer> framebuffer) override;

    void generateCubemap(StringId textureSid, mat4 rotationMatrix = mat4(1.0f)) override;
    void generateProcessedCubemap(GenerateProcessedCubemapInfo gpcInfo) override;
    void generateProcessedTexture(GenerateProcessedTextureInfo gptInfo) override;

    void* getImGuiImage(StringId sid) const override;
};

} // namespace atta::graphics
