// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2020-2026 Breno Cunha Queiroz
#pragma once

#include <atta/graphics/apis/openGL/base.h>
#include <atta/graphics/framebuffer.h>

namespace atta::graphics::gl {

class Framebuffer final : public gfx::Framebuffer {
  public:
    Framebuffer(const Framebuffer::CreateInfo& info);
    ~Framebuffer();

    void bind(bool clear = true) override;
    void unbind() override;

    void resize(uint32_t width, uint32_t height, bool forceRecreate = false) override;

    void setLayer(uint32_t layer) override;

    static GLenum convertDepthAttachmentType(Image::Format format);

  private:
    void createAttachments();
    void bindAttachments();

    OpenGLId _id;
};

} // namespace atta::graphics::gl
