//--------------------------------------------------
// Atta Graphics System
// phongRenderer.cpp
// Date: 2021-09-18
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/graphicsSystem/renderers/phongRenderer.h>
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
#include <atta/componentSystem/components/pointLightComponent.h>
#include <atta/componentSystem/components/directionalLightComponent.h>
#include <atta/componentSystem/factory.h>

namespace atta
{
    PhongRenderer::PhongRenderer():
        Renderer("PhongRenderer")
    {
        //---------- Create geometry pipeline ----------//
        // Framebuffer
        Framebuffer::CreateInfo framebufferInfo {};
        framebufferInfo.attachments.push_back({Image::Format::RGB});
        framebufferInfo.attachments.push_back({Image::Format::DEPTH24_STENCIL8});
        framebufferInfo.width = 500;
        framebufferInfo.height = 500;
        framebufferInfo.debugName = StringId("PhongRenderer Framebuffer");
        std::shared_ptr<Framebuffer> framebuffer = GraphicsManager::create<Framebuffer>(framebufferInfo);

        // Shader Group
        ShaderGroup::CreateInfo shaderGroupInfo {};
        shaderGroupInfo.shaderPaths = {"shaders/phongRenderer/shader.vert", "shaders/phongRenderer/shader.frag"};
        shaderGroupInfo.debugName = StringId("PhongRenderer Shader Group");
        std::shared_ptr<ShaderGroup> shaderGroup = GraphicsManager::create<ShaderGroup>(shaderGroupInfo);

        // Render Pass
        RenderPass::CreateInfo renderPassInfo {};
        renderPassInfo.framebuffer = framebuffer;
        renderPassInfo.debugName = StringId("PhongRenderer Render Pass");
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

    PhongRenderer::~PhongRenderer()
    {
    }

    void PhongRenderer::render(std::shared_ptr<Camera> camera)
    {
        std::vector<EntityId> entities = ComponentManager::getNoPrototypeView();
        _geometryPipeline->begin();
        {
            // Camera
            std::shared_ptr<ShaderGroup> shader = _geometryPipeline->getShaderGroup();
            shader->setMat4("projection", transpose(camera->getProj()));
            shader->setMat4("view", transpose(camera->getView()));
            shader->setVec3("viewPos", camera->getPosition());

            //----- Lighting -----//
            int numPointLights = 0;
            for(auto entity : entities)
            {
                TransformComponent* transform = ComponentManager::getEntityComponent<TransformComponent>(entity);
                PointLightComponent* pl = ComponentManager::getEntityComponent<PointLightComponent>(entity);
                DirectionalLightComponent* dl = ComponentManager::getEntityComponent<DirectionalLightComponent>(entity);

                if(transform && (pl || dl))
                {
                    if(pl && numPointLights < 10)
                    {
                        vec3 position = transform->getWorldTransform(entity).getPosition();
                        int i = numPointLights++;
                        shader->setVec3(("pointLights["+std::to_string(i)+"].position").c_str(), position);
                        shader->setVec3(("pointLights["+std::to_string(i)+"].intensity").c_str(), pl->intensity);
                    }
                    if(dl)
                    {
                        vec3 before = { 0.0f, -1.0f, 0.0f };
                        //vec3 direction;
                        //transform->orientation.transformVector(before, direction);
                        shader->setVec3("directionalLight.direction", before);
                        shader->setVec3("directionalLight.intensity", dl->intensity);
                    }
                    if(numPointLights++ == 10)
                        LOG_WARN("PhongRenderer", "Maximum number of point lights reached, 10 lights");
                }
            }
            shader->setInt("numPointLights", numPointLights);

            // Ambient
            shader->setVec3("ambientColor", {0.2f, 0.2f, 0.2f});
            shader->setFloat("ambientStrength", 1.0f);
            // Diffuse
            shader->setFloat("diffuseStrength", 1.0f);
            // Specular
            shader->setFloat("specularStrength", 1.0f);

            for(auto entity : entities)
            {
                MeshComponent* mesh = ComponentManager::getEntityComponent<MeshComponent>(entity);
                TransformComponent* transform = ComponentManager::getEntityComponent<TransformComponent>(entity);
                MaterialComponent* material = ComponentManager::getEntityComponent<MaterialComponent>(entity);

                if(mesh && transform)
                {
                    mat4 model = transpose(transform->getWorldTransform(entity)); 
                    mat4 invModel = inverse(model);
                    shader->setMat4("model", model);
                    shader->setMat4("invModel", invModel);

                    if(material)
                    {
                        shader->setVec3("material.albedo", material->albedo);
                        shader->setFloat("material.metallic", material->metallic);
                        shader->setFloat("material.roughness", material->roughness);
                        shader->setFloat("material.ao", material->ao);
                    }
                    else
                    {
                        MaterialComponent material {};
                        shader->setVec3("material.albedo", material.albedo);
                        shader->setFloat("material.metallic", material.metallic);
                        shader->setFloat("material.roughness", material.roughness);
                        shader->setFloat("material.ao", material.ao);
                    }

                    GraphicsManager::getRendererAPI()->renderMesh(mesh->sid);
                }
            }

        }
        _geometryPipeline->end();

        _selectedPipeline->render(camera);
        _drawerPipeline->render(camera);
    }

    void PhongRenderer::resize(uint32_t width, uint32_t height)
    {
        if(width != _width || height != _height)
        {
            _geometryPipeline->getRenderPass()->getFramebuffer()->resize(width, height);
            _width = width;
            _height = height;
        }
    }
}
