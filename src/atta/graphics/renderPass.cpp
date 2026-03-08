// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2020-2026 Breno Cunha Queiroz
#include <atta/graphics/renderPass.h>

namespace atta::graphics {

RenderPass::RenderPass(const CreateInfo& info) : _framebuffer(info.framebuffer), _debugName(info.debugName) {}

} // namespace atta::graphics
