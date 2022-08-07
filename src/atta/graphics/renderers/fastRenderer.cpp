//--------------------------------------------------
// Atta Graphics Module
// fastRenderer.cpp
// Date: 2021-09-10
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/graphics/renderers/fastRenderer.h>
#include <atta/graphics/graphicsManager.h>
#include <atta/graphics/framebuffer.h>
#include <atta/graphics/renderPass.h>
#include <atta/graphics/rendererAPIs/openGL/openGLShaderGroup.h>

#include <atta/resource/resourceManager.h>
#include <atta/resource/resources/mesh.h>

#include <atta/component/componentManager.h>
#include <atta/component/components/meshComponent.h>
#include <atta/component/components/transformComponent.h>
#include <atta/component/components/materialComponent.h>
#include <atta/component/factory.h>

namespace atta
{
    FastRenderer::FastRenderer():
        Renderer("FastRenderer")
    {
        //---------- Create geometry pipeline ----------//
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

        //---------- Common pipelines ----------//
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
            std::shared_ptr<ShaderGroup> shader = _geometryPipeline->getShaderGroup();

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
                    {
                        if(material->albedoTexture.getId() != SID("Empty texture"))
                        {
                            shader->setTexture("albedoTexture", material->albedoTexture);
                            shader->setVec3("albedo", {-1, -1, -1});
                        }
                        else
                            shader->setVec3("albedo", material->albedo);
                    }
                    else
                    {
                        MaterialComponent material {};
                        shader->setVec3("albedo", material.albedo);
                    }

                    // Draw mesh
                    GraphicsManager::getRendererAPI()->renderMesh(mesh->sid);
                }
            }
        }
        _geometryPipeline->end();

        if(_renderSelected)
            _selectedPipeline->render(camera);
        if(_renderDrawer)
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
