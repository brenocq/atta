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
        Pipeline::CreateInfo pipelineInfo{};
        pipelineInfo.shader = graphics::create<Shader>("shaders/drawer/line.asl");
        pipelineInfo.renderPass = renderPass;
        pipelineInfo.debugName = StringId("Drawer Line Pipeline");
        _linePipeline = graphics::create<Pipeline>(pipelineInfo);
    }

    //---------- Create point pipeline ----------//
    {
        Pipeline::CreateInfo pipelineInfo{};
        pipelineInfo.shader = graphics::create<Shader>("shaders/drawer/point.asl");
        pipelineInfo.renderPass = renderPass;
        pipelineInfo.debugName = StringId("Drawer Point Pipeline");
        _pointPipeline = graphics::create<Pipeline>(pipelineInfo);
    }
}

void DrawerPipeline::render(std::shared_ptr<Camera> camera) {
    _linePipeline->begin();
    {
        _linePipeline->setMat4("uProjection", camera->getProj());
        _linePipeline->setMat4("uView", camera->getView());
        Drawer::draw<Drawer::Line>();
    }
    _linePipeline->end();

    _pointPipeline->begin();
    {
        _pointPipeline->setMat4("uProjection", camera->getProj());
        _pointPipeline->setMat4("uView", camera->getView());
        Drawer::draw<Drawer::Point>();
    }
    _pointPipeline->end();
}

} // namespace atta::graphics
