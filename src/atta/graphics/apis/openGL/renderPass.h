// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2020-2026 Breno Cunha Queiroz
#pragma once

#include <atta/graphics/renderPass.h>

namespace atta::graphics::gl {

class RenderPass final : public gfx::RenderPass {
  public:
    RenderPass(const RenderPass::CreateInfo& info);
    ~RenderPass();

    void begin(std::shared_ptr<RenderQueue> renderQueue) override;
    void end() override;
};

} // namespace atta::graphics::gl
