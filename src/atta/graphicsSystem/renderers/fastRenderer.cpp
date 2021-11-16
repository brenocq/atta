//--------------------------------------------------
// Atta Graphics System
// fastRenderer.cpp
// Date: 2021-09-10
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/graphicsSystem/renderers/fastRenderer.h>
#include <atta/graphicsSystem/graphicsManager.h>
#include <atta/graphicsSystem/framebuffer.h>
#include <atta/graphicsSystem/renderPass.h>

#include <atta/graphicsSystem/rendererAPIs/openGL/openGLShaderGroup.h>

#include <atta/resourceSystem/resourceManager.h>
#include <atta/resourceSystem/resources/mesh.h>

#include <atta/componentSystem/componentManager.h>
#include <atta/componentSystem/components/meshComponent.h>
#include <atta/componentSystem/components/transformComponent.h>
#include <atta/componentSystem/components/materialComponent.h>
#include <atta/componentSystem/factory.h>

namespace atta
{
    FastRenderer::FastRenderer():
        Renderer("FastRenderer")
    {
        // Framebuffer
        Framebuffer::CreateInfo framebufferInfo {};
        framebufferInfo.attachments.push_back({Image::Format::RGB});
        framebufferInfo.attachments.push_back({Image::Format::DEPTH24_STENCIL8});
        framebufferInfo.width = 500;
        framebufferInfo.height = 500;
        framebufferInfo.debugName = StringId("FastRenderer Framebuffer");
        std::shared_ptr<Framebuffer> framebuffer = GraphicsManager::create<Framebuffer>(framebufferInfo);

        // Shader Group
        ShaderGroup::CreateInfo shaderGroupInfo {};
        shaderGroupInfo.shaderPaths = {"shaders/fastRenderer/shader.vert", "shaders/fastRenderer/shader.frag"};
        shaderGroupInfo.debugName = StringId("FastRenderer Shader Group");
        std::shared_ptr<ShaderGroup> shaderGroup = GraphicsManager::create<ShaderGroup>(shaderGroupInfo);

        // Render Pass
        RenderPass::CreateInfo renderPassInfo {};
        renderPassInfo.framebuffer = framebuffer;
        renderPassInfo.debugName = StringId("FastRenderer Render Pass");
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

        //---------- Selected pipeline ----------//
        _selectedPipeline = std::make_unique<SelectedPipeline>(renderPass, pipelineInfo.layout);
        _drawerPipeline = std::make_unique<DrawerPipeline>(renderPass);
    }

    FastRenderer::~FastRenderer()
    {

    }

    void FastRenderer::render(std::shared_ptr<Camera> camera)
    {
        std::vector<EntityId> entities = ComponentManager::getNoPrototypeView();
        _geometryPipeline->begin();
        {
            std::shared_ptr<OpenGLShaderGroup> shader = std::static_pointer_cast<OpenGLShaderGroup>(_geometryPipeline->getShaderGroup());

            shader->setMat4("projection", transpose(camera->getProj()));
            shader->setMat4("view", transpose(camera->getView()));

            for(auto entity : entities)
            {
                MeshComponent* mesh = ComponentManager::getEntityComponent<MeshComponent>(entity);
                TransformComponent* transform = ComponentManager::getEntityComponent<TransformComponent>(entity);
                MaterialComponent* material = ComponentManager::getEntityComponent<MaterialComponent>(entity);

                if(mesh && transform)
                {
                    mat4 model = transpose(transform->getWorldTransform(entity)); 
                    shader->setMat4("model", model);

                    if(material)
                        shader->setVec3("albedo", material->albedo);
                    else
                    {
                        MaterialComponent material {};
                        shader->setVec3("albedo", material.albedo);
                    }

                    //if(entity == ComponentManager::getSelectedEntity())

                    // Draw mesh
                    GraphicsManager::getRendererAPI()->renderMesh(mesh->sid);
                }
            }
        }
        _geometryPipeline->end();

        _selectedPipeline->render(camera);
        _drawerPipeline->render(camera);
    }

    void FastRenderer::resize(uint32_t width, uint32_t height)
    {
        if(width != _width || height != _height)
        {
            _geometryPipeline->getRenderPass()->getFramebuffer()->resize(width, height);
            _width = width;
            _height = height;
        }
    }
}
