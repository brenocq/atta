//--------------------------------------------------
// Atta Graphics Module
// skyboxPipeline.cpp
// Date: 2025-04-06
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/graphics/interface.h>
#include <atta/graphics/renderers/common/skyboxPipeline.h>

namespace atta::graphics {

SkyboxPipeline::SkyboxPipeline(std::shared_ptr<RenderPass> renderPass) {
    Pipeline::CreateInfo pipelineInfo{};
    pipelineInfo.shader = gfx::create<Shader>("shaders/common/skybox.asl");
    pipelineInfo.renderPass = renderPass;
    pipelineInfo.enableDepthTest = false; // Disable depth testing for the skybox
    pipelineInfo.debugName = StringId("Skybox Pipeline");
    _pipeline = gfx::create<Pipeline>(pipelineInfo);
}

void SkyboxPipeline::render(std::shared_ptr<Camera> camera) {
    _pipeline->begin();
    {
        _pipeline->setMat4("uProjection", camera->getProj());
        _pipeline->setMat4("uView", camera->getView());
        _pipeline->renderCube();
    }
    _pipeline->end();
}

} // namespace atta::graphics
