//--------------------------------------------------
// Atta Graphics Module
// openGLPipeline.cpp
// Date: 2021-09-09
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/graphics/apis/openGL/openGLImage.h>
#include <atta/graphics/apis/openGL/openGLPipeline.h>

namespace atta::graphics {

OpenGLPipeline::OpenGLPipeline(const Pipeline::CreateInfo& info) : Pipeline(info) {
    DASSERT(_renderPass, "Can not create pipeline without render pass");
    DASSERT(_shaderGroup, "Can not create pipeline without shader group");
}

OpenGLPipeline::~OpenGLPipeline() {}

void OpenGLPipeline::begin() {
    _shaderGroup->bind();
    _renderPass->begin();
}

void OpenGLPipeline::end() {
    _renderPass->end();
    //_shaderGroup->unbind();
}

void* OpenGLPipeline::getImGuiTexture() const {
    return reinterpret_cast<void*>(std::static_pointer_cast<OpenGLImage>(_renderPass->getFramebuffer()->getImage(0))->getImGuiImage());
}

} // namespace atta::graphics
