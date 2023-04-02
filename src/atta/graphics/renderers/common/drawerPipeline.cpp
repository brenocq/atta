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
        // Shader Group
        ShaderGroup::CreateInfo shaderGroupInfo{};
        shaderGroupInfo.shaderPaths = {"shaders/line/shader.vert", "shaders/line/shader.frag"};
        shaderGroupInfo.debugName = StringId("DrawerPipeline Line Shader Group");
        std::shared_ptr<ShaderGroup> shaderGroup = graphics::create<ShaderGroup>(shaderGroupInfo);

        Pipeline::CreateInfo pipelineInfo{};
        // Vertex input layout
        pipelineInfo.shaderGroup = shaderGroup;
        pipelineInfo.layout = {{"inPos", VertexBufferElement::Type::VEC3}, {"inColor", VertexBufferElement::Type::VEC4}};
        pipelineInfo.renderPass = renderPass;
        _linePipeline = graphics::create<Pipeline>(pipelineInfo);
    }

    //---------- Create point pipeline ----------//
    {
        // Shader Group
        ShaderGroup::CreateInfo shaderGroupInfo{};
        shaderGroupInfo.shaderPaths = {"shaders/point/shader.vert", "shaders/point/shader.frag"};
        shaderGroupInfo.debugName = StringId("DrawerPipeline Point Shader Group");
        std::shared_ptr<ShaderGroup> shaderGroup = graphics::create<ShaderGroup>(shaderGroupInfo);

        Pipeline::CreateInfo pipelineInfo{};
        // Vertex input layout
        pipelineInfo.shaderGroup = shaderGroup;
        pipelineInfo.layout = {{"inPos", VertexBufferElement::Type::VEC3}, {"inColor", VertexBufferElement::Type::VEC4}};
        pipelineInfo.renderPass = renderPass;
        _pointPipeline = graphics::create<Pipeline>(pipelineInfo);
    }
}

void DrawerPipeline::render(std::shared_ptr<Camera> camera) {
    _linePipeline->begin(false);
    {
        std::shared_ptr<ShaderGroup> shader = _linePipeline->getShaderGroup();
        shader->bind();
        shader->setMat4("projection", transpose(camera->getProj()));
        shader->setMat4("view", transpose(camera->getView()));
        Drawer::draw<Drawer::Line>();
    }
    _linePipeline->end();

    _pointPipeline->begin(false);
    {
        std::shared_ptr<ShaderGroup> shader = _pointPipeline->getShaderGroup();
        shader->bind();
        shader->setMat4("projection", transpose(camera->getProj()));
        shader->setMat4("view", transpose(camera->getView()));
        Drawer::draw<Drawer::Point>();
    }
    _pointPipeline->end();
}

} // namespace atta::graphics
