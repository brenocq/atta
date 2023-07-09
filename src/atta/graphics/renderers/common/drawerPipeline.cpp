//--------------------------------------------------
// Atta Graphics Module
// drawerPipeline.cpp
// Date: 2021-11-16
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/graphics/drawer.h>
#include <atta/graphics/interface.h>
#include <atta/graphics/renderers/common/drawerPipeline.h>

namespace atta::graphics {

DrawerPipeline::DrawerPipeline(std::shared_ptr<RenderPass> renderPass) {
    //---------- Create line pipeline ----------//
    {
        // Shader
        std::shared_ptr<Shader> shader = graphics::create<Shader>("shaders/line/line.asl");

        Pipeline::CreateInfo pipelineInfo{};
        // Vertex input layout
        pipelineInfo.shader = shader;
        pipelineInfo.renderPass = renderPass;
        _linePipeline = graphics::create<Pipeline>(pipelineInfo);
    }

    //---------- Create point pipeline ----------//
    {
        // Shader
        std::shared_ptr<Shader> shader = graphics::create<Shader>("shaders/point/point.asl");

        Pipeline::CreateInfo pipelineInfo{};
        // Vertex input layout
        pipelineInfo.shader= shader;
        pipelineInfo.renderPass = renderPass;
        _pointPipeline = graphics::create<Pipeline>(pipelineInfo);
    }
}

void DrawerPipeline::render(std::shared_ptr<Camera> camera) {
    _linePipeline->begin();
    {
        std::shared_ptr<Shader> shader = _linePipeline->getShader();
        shader->bind();
        shader->setMat4("projection", transpose(camera->getProj()));
        shader->setMat4("view", transpose(camera->getView()));
        Drawer::draw<Drawer::Line>();
    }
    _linePipeline->end();

    _pointPipeline->begin();
    {
        std::shared_ptr<Shader> shader = _pointPipeline->getShader();
        shader->bind();
        shader->setMat4("projection", transpose(camera->getProj()));
        shader->setMat4("view", transpose(camera->getView()));
        Drawer::draw<Drawer::Point>();
    }
    _pointPipeline->end();
}

} // namespace atta::graphics
