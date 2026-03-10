// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2020-2026 Breno Cunha Queiroz
#pragma once

#include <atta/graphics/apis/openGL/base.h>
#include <atta/graphics/image.h>

namespace atta::graphics::gl {

class Image final : public gfx::Image {
  public:
    Image(const Image::CreateInfo& info);
    ~Image();

    static GLenum convertSizedInternalFormat(Format format);
    static GLenum convertFormat(Format format);
    static GLenum convertInternalFormat(Format format);
    static GLenum convertDataType(Format format);
    static GLenum convertSamplerWrap(Wrap samplerWrap);

    void write(uint8_t* data) override;
    std::vector<uint8_t> read(vec2i offset = {0, 0}, vec2i size = {0, 0}) override;
    void resize(uint32_t width, uint32_t height, bool forceRecreate = false) override;

    OpenGLId getHandle() const { return _id; }

    void* getImGuiImage() override { return reinterpret_cast<void*>(OpenGLId(_id)); }

    using FramebufferRead = std::function<std::vector<uint8_t>(vec2i, vec2i)>;
    void setFramebufferRead(std::function<std::vector<uint8_t>(vec2i, vec2i)> framebufferRead);

  private:
    OpenGLId _id;

    /// In OpenGL we need the framebuffer to read the attachment image
    FramebufferRead _framebufferRead;
};

} // namespace atta::graphics::gl
