// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2020-2026 Breno Cunha Queiroz
#include <atta/graphics/apis/openGL/renderPass.h>

namespace atta::graphics::gl {

RenderPass::RenderPass(const RenderPass::CreateInfo& info) : gfx::RenderPass(info) {}

RenderPass::~RenderPass() {}

void RenderPass::begin(std::shared_ptr<RenderQueue> renderQueue) { _framebuffer->bind(true); }

void RenderPass::end() { _framebuffer->unbind(); }

} // namespace atta::graphics::gl
