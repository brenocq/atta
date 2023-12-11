//--------------------------------------------------
// Atta Graphics Module
// pipeline.cpp
// Date: 2021-09-09
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/graphics/apis/openGL/image.h>
#include <atta/graphics/apis/openGL/pipeline.h>

namespace atta::graphics::gl {

Pipeline::Pipeline(const Pipeline::CreateInfo& info) : gfx::Pipeline(info) {
    DASSERT(_renderPass, "Can not create pipeline without render pass");
    DASSERT(_shader, "Can not create pipeline without shader group");
}

Pipeline::~Pipeline() {}

void Pipeline::begin(std::shared_ptr<RenderQueue> renderQueue) { _shader->bind(); }

void Pipeline::end() { _shader->unbind(); }

void Pipeline::resize(uint32_t width, uint32_t height) { _renderPass->getFramebuffer()->resize(width, height); }

void* Pipeline::getImGuiTexture() const {
    return reinterpret_cast<void*>(std::static_pointer_cast<Image>(_renderPass->getFramebuffer()->getImage(0))->getImGuiImage());
}

} // namespace atta::graphics::gl
