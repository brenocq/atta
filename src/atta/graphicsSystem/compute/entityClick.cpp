//--------------------------------------------------
// Atta Graphics System
// entityClick.cpp
// Date: 2021-11-14
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/graphicsSystem/compute/entityClick.h>

#include <atta/graphicsSystem/graphicsManager.h>
#include <atta/graphicsSystem/framebuffer.h>
#include <atta/graphicsSystem/renderPass.h>

#include <atta/componentSystem/componentManager.h>
#include <atta/componentSystem/components/meshComponent.h>
#include <atta/componentSystem/components/transformComponent.h>

namespace atta
{
    EntityClick::EntityClick():
        _width(500), _height(500)
    {
        // Framebuffer
        Framebuffer::CreateInfo framebufferInfo {};
        framebufferInfo.attachments.push_back({Image::Format::RED32I});
        framebufferInfo.attachments.push_back({Image::Format::DEPTH32F});
        framebufferInfo.width = _width;
        framebufferInfo.height = _height;
        framebufferInfo.debugName = StringId("EntityClick Framebuffer");
        std::shared_ptr<Framebuffer> framebuffer = GraphicsManager::create<Framebuffer>(framebufferInfo);

        // Shader Group
        ShaderGroup::CreateInfo shaderGroupInfo {};
        shaderGroupInfo.shaderPaths = {"shaders/compute/entityClick.vert", "shaders/compute/entityClick.frag"};
        shaderGroupInfo.debugName = StringId("EntityClick Shader Group");
        std::shared_ptr<ShaderGroup> shaderGroup = GraphicsManager::create<ShaderGroup>(shaderGroupInfo);

        // Render Pass
        RenderPass::CreateInfo renderPassInfo {};
        renderPassInfo.framebuffer = framebuffer;
        renderPassInfo.debugName = StringId("EntityClick Render Pass");
        std::shared_ptr<RenderPass> renderPass = GraphicsManager::create<RenderPass>(renderPassInfo);

        Pipeline::CreateInfo pipelineInfo {};
        // Vertex input layout
        pipelineInfo.shaderGroup = shaderGroup;
        pipelineInfo.layout = {
            { "inPosition", VertexBufferElement::Type::VEC3 },
            { "inNormal", VertexBufferElement::Type::VEC3 },
            { "inTexCoord", VertexBufferElement::Type::VEC2 }
        };
        pipelineInfo.renderPass = renderPass;
        _geometryPipeline = GraphicsManager::create<Pipeline>(pipelineInfo);
    }

    EntityId EntityClick::click(std::shared_ptr<Viewport> viewport, vec2i pos)
    {
        EntityId eid;
        EntityId maxEid;
        unsigned width = viewport->getWidth();
        unsigned height = viewport->getHeight();
        // Resize
        if(width != _width || height != _height)
        {
            _geometryPipeline->getRenderPass()->getFramebuffer()->resize(width, height);
            _width = width;
            _height = height;
        }

        // Render entity ids
        std::vector<EntityId> entities = ComponentManager::getNoPrototypeView();
        _geometryPipeline->begin();
        {
            std::shared_ptr<OpenGLShaderGroup> shader = std::static_pointer_cast<OpenGLShaderGroup>(_geometryPipeline->getShaderGroup());

            // TODO Clear with -1
            //mat4 m(1.0f);
            //glDisable(GL_DEPTH_TEST);
            //shader->setMat4("model", m);
            //shader->setMat4("projection", m);
            //shader->setMat4("view", m);
            //shader->setInt("entityId", -1);
            //GraphicsManager::getRendererAPI()->renderQuad();
            //glEnable(GL_DEPTH_TEST);

            // Render entities
            shader->setMat4("projection", transpose(viewport->getCamera()->getProj()));
            shader->setMat4("view", transpose(viewport->getCamera()->getView()));

            for(auto entity : entities)
            {
                MeshComponent* mesh = ComponentManager::getEntityComponent<MeshComponent>(entity);
                TransformComponent* transform = ComponentManager::getEntityComponent<TransformComponent>(entity);

                if(mesh && transform)
                {
                    mat4 model; 
                    model.setPosOriScale(transform->position, transform->orientation, transform->scale);
                    model.transpose();
                    shader->setMat4("model", model);

                    // EntityId
                    shader->setInt("entityId", entity);
                    maxEid = std::max((int)maxEid, entity);

                    // Draw mesh
                    GraphicsManager::getRendererAPI()->renderMesh(mesh->sid);
                }
            }

            // Get pixel id
            eid = _geometryPipeline->getRenderPass()->getFramebuffer()->readPixel(0, pos.x, pos.y);
        }
        _geometryPipeline->end();

        return eid > maxEid ? -1 : eid;
    }
}
