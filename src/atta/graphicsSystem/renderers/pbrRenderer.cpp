//--------------------------------------------------
// Atta Graphics System
// pbrRenderer.cpp
// Date: 2021-09-10
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/graphicsSystem/renderers/pbrRenderer.h>
#include <atta/graphicsSystem/graphicsManager.h>
#include <atta/graphicsSystem/framebuffer.h>
#include <atta/graphicsSystem/renderPass.h>

#include <atta/resourceSystem/resourceManager.h>
#include <atta/resourceSystem/resources/mesh.h>

#include <atta/componentSystem/componentManager.h>
#include <atta/componentSystem/components/meshComponent.h>
#include <atta/componentSystem/components/transformComponent.h>
#include <atta/componentSystem/components/materialComponent.h>
#include <atta/componentSystem/components/pointLightComponent.h>
#include <atta/componentSystem/components/directionalLightComponent.h>
#include <atta/componentSystem/components/environmentLightComponent.h>
#include <atta/componentSystem/factory.h>

namespace atta
{
    PbrRenderer::PbrRenderer():
        Renderer("PbrRenderer"), _firstRender(true), _lastEnvironmentMap(StringId("Not defined"))
    {
        // Framebuffer
        Framebuffer::CreateInfo framebufferInfo {};
        framebufferInfo.attachments.push_back({Image::Format::RGB});
        framebufferInfo.attachments.push_back({Image::Format::DEPTH24_STENCIL8});
        framebufferInfo.width = 500;
        framebufferInfo.height = 500;
        framebufferInfo.debugName = StringId("PbrRenderer Framebuffer");
        std::shared_ptr<Framebuffer> framebuffer = GraphicsManager::create<Framebuffer>(framebufferInfo);

        //---------- Create geometry pipeline ----------//
        Pipeline::CreateInfo geometryPipelineInfo {};
        std::shared_ptr<RenderPass> geometryRenderPass;
        {
            // Shader Group
            ShaderGroup::CreateInfo shaderGroupInfo {};
            shaderGroupInfo.shaderPaths = {"shaders/pbrRenderer/shader.vert", "shaders/pbrRenderer/shader.frag"};
            shaderGroupInfo.debugName = StringId("PbrRenderer Shader Group");
            std::shared_ptr<ShaderGroup> shaderGroup = GraphicsManager::create<ShaderGroup>(shaderGroupInfo);

            // Render Pass
            RenderPass::CreateInfo renderPassInfo {};
            renderPassInfo.framebuffer = framebuffer;
            renderPassInfo.debugName = StringId("PbrRenderer Render Pass");
            geometryRenderPass = GraphicsManager::create<RenderPass>(renderPassInfo);

            // Vertex input layout
            geometryPipelineInfo.shaderGroup = shaderGroup;
            geometryPipelineInfo.layout = {
                { "inPosition", VertexBufferElement::Type::VEC3 },
                { "inNormal", VertexBufferElement::Type::VEC3 },
                { "inTexCoord", VertexBufferElement::Type::VEC2 }
            };
            geometryPipelineInfo.renderPass = geometryRenderPass;
            _geometryPipeline = GraphicsManager::create<Pipeline>(geometryPipelineInfo);
        }

        //---------- Common pipelines ----------//
        _selectedPipeline = std::make_unique<SelectedPipeline>(geometryRenderPass, geometryPipelineInfo.layout);
        _drawerPipeline = std::make_unique<DrawerPipeline>(geometryRenderPass);

        //---------- Create background shader ----------//
        {
            ShaderGroup::CreateInfo bgShaderGroupInfo {};
            bgShaderGroupInfo.shaderPaths = {"shaders/pbrRenderer/background.vert", "shaders/pbrRenderer/background.frag"};
            bgShaderGroupInfo.debugName = StringId("PBR background Shader Group");
            _backgroundShader = GraphicsManager::create<ShaderGroup>(bgShaderGroupInfo);
        }

        //---------- Shadow mapping ----------//
        {
            // Framebuffer
            Framebuffer::CreateInfo framebufferInfo {};
            framebufferInfo.attachments.push_back({Image::Format::DEPTH32F});
            framebufferInfo.width = 1024;
            framebufferInfo.height = 1024;
            framebufferInfo.debugName = StringId("PbrRenderer::shadowMap::framebuffer");
            std::shared_ptr<Framebuffer> framebuffer = GraphicsManager::create<Framebuffer>(framebufferInfo);

            // Shader Group
            ShaderGroup::CreateInfo shaderGroupInfo {};
            shaderGroupInfo.shaderPaths = {"shaders/pbrRenderer/shadow.vert", "shaders/pbrRenderer/shadow.frag"};
            shaderGroupInfo.debugName = StringId("PbrRenderer::shadowMap::shaderGroup");
            std::shared_ptr<ShaderGroup> shaderGroup = GraphicsManager::create<ShaderGroup>(shaderGroupInfo);

            // Render Pass
            RenderPass::CreateInfo renderPassInfo {};
            renderPassInfo.framebuffer = framebuffer;
            renderPassInfo.debugName = StringId("PbrRenderer::shadowMap::renderPass");
            std::shared_ptr<RenderPass> renderPass = GraphicsManager::create<RenderPass>(renderPassInfo);

            // Vertex input layout
            Pipeline::CreateInfo pipelineInfo {};
            pipelineInfo.shaderGroup = shaderGroup;
            pipelineInfo.renderPass = renderPass;
            pipelineInfo.layout = {
                { "inPosition", VertexBufferElement::Type::VEC3 },
                { "inNormal", VertexBufferElement::Type::VEC3 },
                { "inTexCoord", VertexBufferElement::Type::VEC2 }
            };
            _shadowMapPipeline = GraphicsManager::create<Pipeline>(pipelineInfo);
        }
    }

    PbrRenderer::~PbrRenderer()
    {
    }

    void PbrRenderer::render(std::shared_ptr<Camera> camera)
    {
        if(_firstRender)
        {
            brdfLUT();
            _firstRender = false;
        }

        std::vector<EntityId> entities = ComponentManager::getNoPrototypeView();

        // Generate envmap if necessary
        bool foundEnvMap = false;
        for(auto entity : entities)
        {
            EnvironmentLightComponent* el = ComponentManager::getEntityComponent<EnvironmentLightComponent>(entity);

            if(el)
            {
                foundEnvMap = true;
                if(el->sid != _lastEnvironmentMap)
                {
                    _lastEnvironmentMap = el->sid;
                    GraphicsManager::getRendererAPI()->generateCubemap(_lastEnvironmentMap);
                    irradianceCubemap();
                    prefilterCubemap();
                }
            }
        }
        if(!foundEnvMap)
            _lastEnvironmentMap = StringId("Not defined");

        shadowPass();
        geometryPass(camera);

        _selectedPipeline->render(camera);
        _drawerPipeline->render(camera);
    }

    void PbrRenderer::resize(uint32_t width, uint32_t height)
    {
        if(width != _width || height != _height)
        {
            _geometryPipeline->getRenderPass()->getFramebuffer()->resize(width, height);
            _width = width;
            _height = height;
        }
    }

    void PbrRenderer::shadowPass()
    {
        std::vector<EntityId> entities = ComponentManager::getNoPrototypeView();

        //----- Directional shadow mapping -----//
        EntityId directionalLightEntity = -1;
        for(auto entity : entities)
        {
            DirectionalLightComponent* dl = ComponentManager::getEntityComponent<DirectionalLightComponent>(entity);
            if(dl)
            {
                directionalLightEntity = entity;
                break;
            }
        }

        if(directionalLightEntity != -1)
        {
            _shadowMapPipeline->begin();
            {
                std::shared_ptr<ShaderGroup> shader = _shadowMapPipeline->getShaderGroup();
                shader->bind();

                // Create light matrix
                DirectionalLightComponent* dl = ComponentManager::getEntityComponent<DirectionalLightComponent>(directionalLightEntity);
                TransformComponent* t = ComponentManager::getEntityComponent<TransformComponent>(directionalLightEntity);

                float height = 10.0f;
                float ratio = 1.0f;
                float far = 10.0f;
                mat4 proj = orthographic(height, ratio, far);
                mat4 view;
                view.setPosOri(vec3(), t->orientation);
                _directionalLightMatrix = proj*view;
                shader->setMat4("lightSpaceMatrix", transpose(_directionalLightMatrix));

                // Fill shadow map rendering the scene
                for(auto entity : entities)
                {
                    MeshComponent* mesh = ComponentManager::getEntityComponent<MeshComponent>(entity);
                    TransformComponent* transform = ComponentManager::getEntityComponent<TransformComponent>(entity);

                    if(mesh && transform)
                    {
                        shader->setMat4("model", transpose(transform->getWorldTransform(entity)));
                        GraphicsManager::getRendererAPI()->renderMesh(mesh->sid);
                    }
                }
            }
            _shadowMapPipeline->end();
        }
    }

    void PbrRenderer::geometryPass(std::shared_ptr<Camera> camera)
    {
        std::vector<EntityId> entities = ComponentManager::getNoPrototypeView();
        _geometryPipeline->begin();
        {
            //---------- PBR shader ----------//
            std::shared_ptr<ShaderGroup> shader = _geometryPipeline->getShaderGroup();
            shader->bind();

            shader->setMat4("projection", transpose(camera->getProj()));
            shader->setMat4("view", transpose(camera->getView()));
            shader->setVec3("camPos", camera->getPosition());
            shader->setMat4("directionalLightMatrix", transpose(_directionalLightMatrix));
            shader->setTexture("directionalShadowMap", _shadowMapPipeline->getRenderPass()->getFramebuffer()->getImage());

            if(_lastEnvironmentMap != "Not defined"_sid)
            {
                shader->setCubemap("irradianceMap", "PbrRenderer::irradiance");
                shader->setCubemap("prefilterMap", "PbrRenderer::prefilter");
                shader->setTexture("brdfLUT", "PbrRenderer::brdfLUT");
                shader->setInt("numEnvironmentLights", 1);
                shader->setMat3("environmentLightOri", mat3(1.0f));
            }
            else
                shader->setInt("numEnvironmentLights", 0);


            //----- Lighting -----//
            _environmentMapOri = mat3(1.0f);
            int numPointLights = 0;
            int numDirectionalLights = 0;
            for(auto entity : entities)
            {
                TransformComponent* transform = ComponentManager::getEntityComponent<TransformComponent>(entity);
                PointLightComponent* pl = ComponentManager::getEntityComponent<PointLightComponent>(entity);
                DirectionalLightComponent* dl = ComponentManager::getEntityComponent<DirectionalLightComponent>(entity);
                EnvironmentLightComponent* el = ComponentManager::getEntityComponent<EnvironmentLightComponent>(entity);

                if(transform && (pl || dl || el))
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
                        vec3 base = { 0.0f, 0.0f, -1.0f };
                        mat4 rotation;
                        rotation.setPosOri({0,0,0}, transform->orientation);
                        base = transpose(mat3(rotation)) * base;
                        shader->setVec3("directionalLight.direction", base);
                        shader->setVec3("directionalLight.intensity", dl->intensity);
                        numDirectionalLights++;
                    }
                    if(el)
                    {
                        mat4 ori;
                        ori.setPosOri(vec3(0.0f), transform->orientation);
                        _environmentMapOri = mat3(ori);
                        shader->setMat3("environmentLightOri", transpose(_environmentMapOri));
                    }
                    if(numPointLights++ == 10)
                        LOG_WARN("PbrRenderer", "Maximum number of point lights reached, 10 lights");
                }
            }
            shader->setInt("numPointLights", numPointLights);
            shader->setInt("numDirectionalLights", numDirectionalLights ? 1 : 0);

            //----- Entities -----//
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
                        if(material->albedoTexture.getId() != SID("Empty texture"))
                        {
                            shader->setTexture("albedoTexture", material->albedoTexture);
                            shader->setVec3("material.albedo", {-1, -1, -1});
                        }
                        else
                            shader->setVec3("material.albedo", material->albedo);

                        if(material->metallicTexture.getId() != SID("Empty texture"))
                        {
                            shader->setTexture("metallicTexture", material->metallicTexture);
                            shader->setFloat("material.metallic", -1);
                        }
                        else
                            shader->setFloat("material.metallic", material->metallic);

                        if(material->roughnessTexture.getId() != SID("Empty texture"))
                        {
                            shader->setTexture("roughnessTexture", material->roughnessTexture);
                            shader->setFloat("material.roughness", -1);
                        }
                        else
                            shader->setFloat("material.roughness", material->roughness);

                        if(material->aoTexture.getId() != SID("Empty texture"))
                        {
                            shader->setTexture("aoTexture", material->aoTexture);
                            shader->setFloat("material.ao", -1);
                        }
                        else
                            shader->setFloat("material.ao", material->ao);

                        if(material->normalTexture.getId() != SID("Empty texture"))
                        {
                            shader->setTexture("normalTexture", material->normalTexture);
                            shader->setInt("material.hasNormalTexture", 1);
                        }
                        else
                            shader->setInt("material.hasNormalTexture", 0);
                    }
                    else
                    {
                        MaterialComponent material {};
                        //shader->setTexture("albedoTexture", _shadowMapPipeline->getRenderPass()->getFramebuffer()->getImage());
                        //shader->setVec3("material.albedo", {-1, -1, -1});
                        shader->setVec3("material.albedo", material.albedo);
                        shader->setFloat("material.metallic", material.metallic);
                        shader->setFloat("material.roughness", material.roughness);
                        shader->setFloat("material.ao", material.ao);
                        shader->setInt("material.hasNormalTexture", 0);
                    }

                    GraphicsManager::getRendererAPI()->renderMesh(mesh->sid);
                }
            }

            //---------- Background shader ----------//
            if(_lastEnvironmentMap != "Not defined"_sid)
            {
                _backgroundShader->bind();
                _backgroundShader->setMat4("projection", transpose(camera->getProj()));
                _backgroundShader->setMat4("view", transpose(camera->getView()));
                _backgroundShader->setCubemap("environmentMap", _lastEnvironmentMap);
                //_backgroundShader->setCubemap("environmentMap", "PbrRenderer::irradiance");
                _backgroundShader->setMat3("environmentMapOri", _environmentMapOri);

                GraphicsManager::getRendererAPI()->renderCube();
            }

        }
        _geometryPipeline->end();
    }

    void PbrRenderer::irradianceCubemap()
    {
        // Create shader
        ShaderGroup::CreateInfo shaderGroupInfo {};
        shaderGroupInfo.shaderPaths = {"shaders/compute/irradiance.vert", "shaders/compute/irradiance.frag"};
        shaderGroupInfo.debugName = StringId("Irradiance Shader Group");
        std::shared_ptr<ShaderGroup> shader = GraphicsManager::create<ShaderGroup>(shaderGroupInfo);

        // Generate irradiance cubemap
        RendererAPI::GenerateProcessedCubemapInfo info;
        info.cubemapSid = StringId("PbrRenderer::irradiance");
        info.shader = shader;
        info.width = 128;
        info.height = 128;
        info.numMipLevels = 1;
        info.func = [&](std::shared_ptr<ShaderGroup> shader, mat4 proj, mat4 view, int face, int mipLevel) 
            {
                if(mipLevel == 0 && face == 0)
                {
                    shader->setCubemap("environmentMap", _lastEnvironmentMap);
                    shader->setMat4("projection", transpose(proj));
                }
                shader->setMat4("view", transpose(view));
            };
        GraphicsManager::getRendererAPI()->generateProcessedCubemap(info);
    }

    void PbrRenderer::prefilterCubemap()
    {
        // Create shader
        ShaderGroup::CreateInfo shaderGroupInfo {};
        shaderGroupInfo.shaderPaths = {"shaders/compute/prefilter.vert", "shaders/compute/prefilter.frag"};
        shaderGroupInfo.debugName = StringId("Prefilter Shader Group");
        std::shared_ptr<ShaderGroup> shader = GraphicsManager::create<ShaderGroup>(shaderGroupInfo);

        // Generate prefilter cubemap
        RendererAPI::GenerateProcessedCubemapInfo info;
        info.cubemapSid = StringId("PbrRenderer::prefilter");
        info.shader = shader;
        info.width = 512;
        info.height = 512;
        info.numMipLevels = 5;
        info.func = [&](std::shared_ptr<ShaderGroup> shader, mat4 proj, mat4 view, int face, int mipLevel) 
            {
                if(mipLevel == 0 && face == 0)
                {
                    shader->setCubemap("environmentMap", _lastEnvironmentMap);
                    shader->setMat4("projection", transpose(proj));
                }

                shader->setMat4("view", transpose(view));

                float roughness = (float)mipLevel / (float)(info.numMipLevels - 1);
                shader->setFloat("roughness", roughness);
            };
        GraphicsManager::getRendererAPI()->generateProcessedCubemap(info);
    }

    void PbrRenderer::brdfLUT()
    {
        ShaderGroup::CreateInfo shaderGroupInfo {};
        shaderGroupInfo.shaderPaths = {"shaders/compute/brdf.vert", "shaders/compute/brdf.frag"};
        shaderGroupInfo.debugName = StringId("BRDF LUT Shader Group");
        std::shared_ptr<ShaderGroup> shader = GraphicsManager::create<ShaderGroup>(shaderGroupInfo);

        // Generate brdf LUT
        RendererAPI::GenerateProcessedTextureInfo info;
        info.textureSid = StringId("PbrRenderer::brdfLUT");
        info.shader = shader;
        info.imageInfo.format = Image::Format::RG16F;
        info.imageInfo.samplerWrap = Image::Wrap::REPEAT;
        info.imageInfo.width = 512;
        info.imageInfo.height = 512;
        info.imageInfo.mipLevels = 1;
        info.imageInfo.debugName = StringId("PbrRenderer brdfLUT image");

        GraphicsManager::getRendererAPI()->generateProcessedTexture(info);
    }
}
