//--------------------------------------------------
// Atta Graphics System
// drawerPipeline.cpp
// Date: 2021-11-16
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/graphicsSystem/renderers/common/drawerPipeline.h>
#include <atta/graphicsSystem/graphicsManager.h>
#include <atta/graphicsSystem/rendererAPIs/openGL/openGLShaderGroup.h>
#include <atta/graphicsSystem/drawer.h>

namespace atta
{
    DrawerPipeline::DrawerPipeline(std::shared_ptr<RenderPass> renderPass)
    {
        //---------- Create line pipeline ----------//
        {
            // Shader Group
            ShaderGroup::CreateInfo shaderGroupInfo {};
            shaderGroupInfo.shaderPaths = {"shaders/line/shader.vert", "shaders/line/shader.frag"};
            shaderGroupInfo.debugName = StringId("DrawerPipeline Line Shader Group");
            std::shared_ptr<ShaderGroup> shaderGroup = GraphicsManager::create<ShaderGroup>(shaderGroupInfo);

            Pipeline::CreateInfo pipelineInfo {};
            // Vertex input layout
            pipelineInfo.shaderGroup = shaderGroup;
            pipelineInfo.layout = {
                { "inPos", VertexBufferElement::Type::VEC3 },
                { "inColor", VertexBufferElement::Type::VEC4 }
            };
            pipelineInfo.renderPass = renderPass;
            _linePipeline = GraphicsManager::create<Pipeline>(pipelineInfo);
        }

        //---------- Create point pipeline ----------//
        {
            // Shader Group
            ShaderGroup::CreateInfo shaderGroupInfo {};
            shaderGroupInfo.shaderPaths = {"shaders/point/shader.vert", "shaders/point/shader.frag"};
            shaderGroupInfo.debugName = StringId("DrawerPipeline Point Shader Group");
            std::shared_ptr<ShaderGroup> shaderGroup = GraphicsManager::create<ShaderGroup>(shaderGroupInfo);

            Pipeline::CreateInfo pipelineInfo {};
            // Vertex input layout
            pipelineInfo.shaderGroup = shaderGroup;
            pipelineInfo.layout = {
                { "inPos", VertexBufferElement::Type::VEC3 },
                { "inColor", VertexBufferElement::Type::VEC4 }
            };
            pipelineInfo.renderPass = renderPass;
            _pointPipeline = GraphicsManager::create<Pipeline>(pipelineInfo);
        }
    }

    void DrawerPipeline::render(std::shared_ptr<Camera> camera)
    {
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
}
