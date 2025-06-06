//--------------------------------------------------
// Atta Graphics Module
// pbrRenderer.cpp
// Date: 2021-09-10
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/graphics/renderers/pbrRenderer.h>

#include <atta/graphics/framebuffer.h>
#include <atta/graphics/interface.h>
#include <atta/graphics/renderPass.h>

#include <atta/resource/interface.h>
#include <atta/resource/resources/mesh.h>

#include <atta/component/components/directionalLight.h>
#include <atta/component/components/environmentLight.h>
#include <atta/component/components/material.h>
#include <atta/component/components/mesh.h>
#include <atta/component/components/pointLight.h>
#include <atta/component/components/transform.h>
#include <atta/component/factory.h>
#include <atta/component/interface.h>

namespace atta::graphics {

PbrRenderer::PbrRenderer() : Renderer("PbrRenderer"), _firstRender(true), _wasResized(false), _lastEnvironmentMap(StringId("Not defined")) {
    // Render Queue
    _renderQueue = graphics::create<RenderQueue>();

    //---------- Create geometry pipeline ----------//
    std::shared_ptr<RenderPass> geometryRenderPass;
    {
        // Framebuffer
        Framebuffer::CreateInfo framebufferInfo{};
        framebufferInfo.attachments.push_back({Image::Format::RGBA});
        framebufferInfo.attachments.push_back({Image::Format::DEPTH24_STENCIL8});
        _width = framebufferInfo.width = 500;
        _height = framebufferInfo.height = 500;
        framebufferInfo.clearColor = {0.2f, 0.2f, 0.2f, 1.0f};
        framebufferInfo.debugName = StringId("PbrRenderer Framebuffer");

        // Render Pass
        RenderPass::CreateInfo renderPassInfo{};
        renderPassInfo.framebuffer = graphics::create<Framebuffer>(framebufferInfo);
        renderPassInfo.debugName = StringId("PbrRenderer Render Pass");
        _geometryRenderPass = graphics::create<RenderPass>(renderPassInfo);

        // Pipeline
        Pipeline::CreateInfo geometryPipelineInfo{};
        geometryPipelineInfo.shader = graphics::create<Shader>("shaders/pbrRenderer/pbrRenderer.asl");
        geometryPipelineInfo.renderPass = _geometryRenderPass;
        geometryPipelineInfo.debugName = StringId("PbrRenderer Pipeline");
        _geometryPipeline = graphics::create<Pipeline>(geometryPipelineInfo);
    }

    //---------- Common pipelines ----------//
    //_selectedPipeline = std::make_unique<SelectedPipeline>(_geometryRenderPass);
    _gridPipeline = std::make_unique<GridPipeline>(_geometryRenderPass);
    _drawerPipeline = std::make_unique<DrawerPipeline>(_geometryRenderPass);

    ////---------- Create background shader ----------//
    //{ _backgroundShader = graphics::create<Shader>("shaders/pbrRenderer/background.asl"); }

    ////---------- Directional shadow mapping ----------//
    //{
    //    // Framebuffer
    //    Image::CreateInfo imageInfo;
    //    imageInfo.format = Image::Format::DEPTH32F;
    //    imageInfo.samplerWrap = Image::Wrap::BORDER;
    //    imageInfo.borderColor = vec4(1.0f);
    //    imageInfo.width = 1024;
    //    imageInfo.height = 1024;
    //    imageInfo.debugName = StringId("PbrRenderer::dirShadowMap::image");
    //    std::shared_ptr<Image> depthImage = graphics::create<Image>(imageInfo);

    //    Framebuffer::CreateInfo framebufferInfo{};
    //    framebufferInfo.attachments.push_back({Image::Format::NONE, depthImage});
    //    framebufferInfo.width = 1024;
    //    framebufferInfo.height = 1024;
    //    framebufferInfo.debugName = StringId("PbrRenderer::shadowMap::framebuffer");
    //    std::shared_ptr<Framebuffer> framebuffer = graphics::create<Framebuffer>(framebufferInfo);

    //    // Shader
    //    std::shared_ptr<Shader> shader = graphics::create<Shader>("shaders/pbrRenderer/shadow.asl");

    //    // Render Pass
    //    RenderPass::CreateInfo renderPassInfo{};
    //    renderPassInfo.framebuffer = framebuffer;
    //    renderPassInfo.debugName = StringId("PbrRenderer::shadowMap::renderPass");
    //    std::shared_ptr<RenderPass> renderPass = graphics::create<RenderPass>(renderPassInfo);

    //    // Pipeline
    //    Pipeline::CreateInfo pipelineInfo{};
    //    pipelineInfo.shader = shader;
    //    pipelineInfo.renderPass = renderPass;
    //    _shadowMapPipeline = graphics::create<Pipeline>(pipelineInfo);
    //}

    ////---------- Omnidirectional shadow mapping ----------//
    //{
    //    // Framebuffer
    //    Image::CreateInfo imageInfo;
    //    imageInfo.format = Image::Format::DEPTH32F;
    //    imageInfo.width = 1024;
    //    imageInfo.height = 1024;
    //    imageInfo.isCubemap = true;
    //    imageInfo.debugName = StringId("PbrRenderer::omniShadowMap::image");
    //    _omnidirectionalShadowMap = graphics::create<Image>(imageInfo);

    //    Framebuffer::CreateInfo framebufferInfo{};
    //    framebufferInfo.attachments.push_back({Image::Format::NONE, _omnidirectionalShadowMap});
    //    framebufferInfo.width = 1024;
    //    framebufferInfo.height = 1024;
    //    framebufferInfo.debugName = StringId("PbrRenderer::omniShadowMap::framebuffer");
    //    std::shared_ptr<Framebuffer> framebuffer = graphics::create<Framebuffer>(framebufferInfo);

    //    // Shader
    //    std::shared_ptr<Shader> shader = graphics::create<Shader>("shaders/pbrRenderer/omniShadow.asl");

    //    // Render Pass
    //    RenderPass::CreateInfo renderPassInfo{};
    //    renderPassInfo.framebuffer = framebuffer;
    //    renderPassInfo.debugName = StringId("PbrRenderer::omniShadowMap::renderPass");
    //    std::shared_ptr<RenderPass> renderPass = graphics::create<RenderPass>(renderPassInfo);

    //    // Pipeline
    //    Pipeline::CreateInfo pipelineInfo{};
    //    pipelineInfo.shader = shader;
    //    pipelineInfo.renderPass = renderPass;
    //    _omniShadowMapPipeline = graphics::create<Pipeline>(pipelineInfo);
    //}
}

PbrRenderer::~PbrRenderer() {}

void PbrRenderer::render(std::shared_ptr<Camera> camera) {
    if (_wasResized) {
        _geometryPipeline->resize(_width, _height);
        _wasResized = false;
    }
    // if (_firstRender)
    //     brdfLUT();

    // Check current envinroment map
    // std::vector<component::EntityId> entities = component::getNoPrototypeView();
    // StringId currEnvironmentMap = StringId("textures/white.png"); // Default environment map is white texture
    // for (auto entity : entities) {
    //    component::EnvironmentLight* el = component::getComponent<component::EnvironmentLight>(entity);

    //    if (el) {
    //        currEnvironmentMap = el->sid;
    //        break;
    //    }
    //}

    // Recreate environment map if it is different from last one
    // if (currEnvironmentMap != _lastEnvironmentMap) {
    //    _lastEnvironmentMap = currEnvironmentMap;
    //    graphics::getGraphicsAPI()->generateCubemap(_lastEnvironmentMap);
    //    irradianceCubemap();
    //    prefilterCubemap();
    //}

    // shadowPass();
    geometryPass(camera);

    // if (_renderSelected)
    //     _selectedPipeline->render(camera);
    // if (_renderDrawer)
    //     _drawerPipeline->render(camera);

    _firstRender = false;
}

void PbrRenderer::resize(uint32_t width, uint32_t height) {
    if (width != _width || height != _height) {
        _wasResized = true;
        _width = width;
        _height = height;
    }
}

void PbrRenderer::shadowPass() {
    // std::vector<component::EntityId> entities = component::getNoPrototypeView();

    ////----- Directional shadow mapping -----//
    // component::EntityId directionalLightEntity = -1;
    // for (auto entity : entities) {
    //     component::DirectionalLight* dl = component::getComponent<component::DirectionalLight>(entity);
    //     component::Transform* t = component::getComponent<component::Transform>(entity);
    //     if (dl && t) {
    //         directionalLightEntity = entity;
    //         break;
    //     }
    // }

    // if (directionalLightEntity != -1) {
    //     _shadowMapPipeline->begin();
    //     {
    //         std::shared_ptr<Shader> shader = _shadowMapPipeline->getShader();
    //         shader->bind();

    //        // Create light matrix
    //        component::Transform* t = component::getComponent<component::Transform>(directionalLightEntity);

    //        float height = 10.0f;
    //        float ratio = 1.0f;
    //        float far = 25.0f;
    //        mat4 proj = orthographic(height, ratio, far);
    //        mat4 view;
    //        view.setPosOri(vec3(), t->orientation);
    //        _directionalLightMatrix = proj * view;
    //        shader->setMat4("lightSpaceMatrix", transpose(_directionalLightMatrix));

    //        // Fill shadow map rendering the scene
    //        for (auto entity : entities) {
    //            component::Mesh* mesh = component::getComponent<component::Mesh>(entity);
    //            component::Transform* transform = component::getComponent<component::Transform>(entity);

    //            if (mesh && transform) {
    //                shader->setMat4("model", transpose(transform->getWorldTransformMatrix(entity)));
    //                graphics::getGraphicsAPI()->renderMesh(mesh->sid);
    //            }
    //        }
    //    }
    //    _shadowMapPipeline->end();
    //}

    ////----- Omnidirectional shadow mapping -----//
    // component::EntityId pointLightEntity = -1;
    // for (auto entity : entities) {
    //     component::PointLight* pl = component::getComponent<component::PointLight>(entity);
    //     component::Transform* t = component::getComponent<component::Transform>(entity);
    //     if (pl && t) {
    //         pointLightEntity = entity;
    //         break;
    //     }
    // }

    // if (pointLightEntity != -1) {
    //     _omniShadowMapPipeline->begin();
    //     {
    //         std::shared_ptr<Shader> shader = _omniShadowMapPipeline->getShader();
    //         shader->bind();

    //        // Create light matrix
    //        component::Transform* t = component::getComponent<component::Transform>(pointLightEntity);

    //        // TODO world position
    //        float fov = radians(90.0f);
    //        float ratio = 1.0f;
    //        float near = 0.01f;
    //        float far = 25.0f;
    //        mat4 proj = perspective(fov, ratio, near, far);
    //        std::vector<mat4> shadowMatrices = {proj * lookAt(t->position, t->position + vec3(1.0f, 0.0f, 0.0f), vec3(0.0f, -1.0f, 0.0f)),
    //                                            proj * lookAt(t->position, t->position + vec3(-1.0f, 0.0f, 0.0f), vec3(0.0f, -1.0f, 0.0f)),
    //                                            proj * lookAt(t->position, t->position + vec3(0.0f, 1.0f, 0.0f), vec3(0.0f, 0.0f, 1.0f)),
    //                                            proj * lookAt(t->position, t->position + vec3(0.0f, -1.0f, 0.0f), vec3(0.0f, 0.0f, -1.0f)),
    //                                            proj * lookAt(t->position, t->position + vec3(0.0f, 0.0f, 1.0f), vec3(0.0f, -1.0f, 0.0f)),
    //                                            proj * lookAt(t->position, t->position + vec3(0.0f, 0.0f, -1.0f), vec3(0.0f, -1.0f, 0.0f))};

    //        shader->setMat4("shadowMatrices[0]", transpose(shadowMatrices[0]));
    //        shader->setMat4("shadowMatrices[1]", transpose(shadowMatrices[1]));
    //        shader->setMat4("shadowMatrices[2]", transpose(shadowMatrices[2]));
    //        shader->setMat4("shadowMatrices[3]", transpose(shadowMatrices[3]));
    //        shader->setMat4("shadowMatrices[4]", transpose(shadowMatrices[4]));
    //        shader->setMat4("shadowMatrices[5]", transpose(shadowMatrices[5]));
    //        shader->setMat4("model", transpose(t->getWorldTransformMatrix(pointLightEntity)));
    //        shader->setVec3("lightPos", t->position);
    //        shader->setFloat("far_plane", far);

    //        // Fill shadow map rendering the scene
    //        for (auto entity : entities) {
    //            component::Mesh* mesh = component::getComponent<component::Mesh>(entity);
    //            component::Transform* transform = component::getComponent<component::Transform>(entity);

    //            if (mesh && transform) {
    //                shader->setMat4("model", transpose(transform->getWorldTransformMatrix(entity)));
    //                graphics::getGraphicsAPI()->renderMesh(mesh->sid);
    //            }
    //        }
    //    }
    //    _omniShadowMapPipeline->end();
    //}
}

void PbrRenderer::geometryPass(std::shared_ptr<Camera> camera) {
    // Handle image group update from materials
    _geometryPipeline->updateImageGroupsFromMaterials([](resource::Material* material) {
        Pipeline::ImageGroup imageGroup;
        if (material->colorIsImage())
            imageGroup.emplace_back("albedoTexture", material->getColorImage());
        if (material->metallicIsImage())
            imageGroup.emplace_back("metallicTexture", material->getMetallicImage());
        if (material->roughnessIsImage())
            imageGroup.emplace_back("roughnessTexture", material->getRoughnessImage());
        if (material->aoIsImage())
            imageGroup.emplace_back("aoTexture", material->getAoImage());
        return imageGroup;
    });

    // Update grid data
    _gridPipeline->update(camera);

    // Update drawer data
    if (_renderDrawer)
        _drawerPipeline->update();

    // Render
    _renderQueue->begin();
    {
        _geometryRenderPass->begin(_renderQueue);
        {
            _geometryPipeline->begin();
            {
                //---------- PBR shader ----------//
                _geometryPipeline->setMat4("projection", camera->getProj());
                _geometryPipeline->setMat4("view", camera->getView());
                _geometryPipeline->setVec3("camPos", camera->getPosition());
                _geometryPipeline->setMat4("directionalLightMatrix", _directionalLightMatrix);
                // _geometryPipeline->setImage("directionalShadowMap", _shadowMapPipeline->getRenderPass()->getFramebuffer()->getImage());
                // _geometryPipeline->setCubemap("omniShadowMap", _omnidirectionalShadowMap);
                _geometryPipeline->setFloat("omniFarPlane", 25.0f);

                // Always set environment textures (if there is no environment map, use white texture)
                // _geometryPipeline->setCubemap("irradianceMap", "PbrRenderer::irradiance");
                // _geometryPipeline->setCubemap("prefilterMap", "PbrRenderer::prefilter");
                //_geometryPipeline->setImage("brdfLUT", "PbrRenderer::brdfLUT");

                // if (_lastEnvironmentMap != "Not defined"_sid)
                //     _geometryPipeline->setInt("numEnvironmentLights", 1);
                // else {
                //     LOG_WARN("graphics::PbrRenderer", "Number of environment light should always be 1 (white texture if not defined)");
                //     _geometryPipeline->setInt("numEnvironmentLights", 0);
                // }
                _geometryPipeline->setMat3("environmentLightOri", mat3(1.0f));

                //----- Lighting -----//
                _environmentMapOri = mat3(1.0f);
                int numPointLights = 0;
                int numDirectionalLights = 0;
                std::vector<component::EntityId> entities = component::getNoPrototypeView();
                for (auto entity : entities) {
                    component::Transform* transform = component::getComponent<component::Transform>(entity);
                    component::PointLight* pl = component::getComponent<component::PointLight>(entity);
                    component::DirectionalLight* dl = component::getComponent<component::DirectionalLight>(entity);
                    component::EnvironmentLight* el = component::getComponent<component::EnvironmentLight>(entity);

                    if (transform && (pl || dl || el)) {
                        if (pl && numPointLights < 10) {
                            vec3 position = transform->getWorldTransformMatrix(entity).getPosition();
                            int i = numPointLights++;
                            _geometryPipeline->setVec3(("pointLights[" + std::to_string(i) + "].position").c_str(), position);
                            _geometryPipeline->setVec3(("pointLights[" + std::to_string(i) + "].intensity").c_str(), pl->intensity);
                        }
                        if (dl) {
                            vec3 base = {0.0f, 0.0f, -1.0f};
                            mat4 rotation;
                            rotation.setPosOri({0, 0, 0}, transform->orientation);
                            base = mat3(rotation) * base;
                            _geometryPipeline->setVec3("directionalLight.direction", base);
                            _geometryPipeline->setVec3("directionalLight.intensity", dl->intensity);
                            numDirectionalLights++;
                        }
                        if (el) {
                            mat4 ori;
                            ori.setPosOri(vec3(0.0f), transform->orientation);
                            _environmentMapOri = mat3(ori);
                            _geometryPipeline->setMat3("environmentLightOri", transpose(_environmentMapOri));
                        }
                        if (numPointLights++ == 10)
                            LOG_WARN("gfx::PbrRenderer", "Maximum number of point lights reached, 10 lights");
                    }
                }
                _geometryPipeline->setInt("numPointLights", numPointLights);
                _geometryPipeline->setInt("numDirectionalLights", numDirectionalLights ? 1 : 0);

                //----- Meshes -----//
                for (auto entity : entities) {
                    component::Mesh* mesh = component::getComponent<component::Mesh>(entity);
                    component::Transform* transform = component::getComponent<component::Transform>(entity);
                    component::Material* compMat = component::getComponent<component::Material>(entity);
                    resource::Material* material = compMat ? compMat->getResource() : nullptr;

                    if (mesh && transform) {
                        mat4 model = transform->getWorldTransformMatrix(entity);
                        _geometryPipeline->setMat4("model", model);
                        _geometryPipeline->setMat4("invModel", inverse(model));

                        if (material) {
                            _geometryPipeline->setImageGroup(compMat->sid);
                            _geometryPipeline->setVec3("material.albedo", material->colorIsImage() ? vec3(-1, -1, -1) : material->getColor());
                            _geometryPipeline->setFloat("material.metallic", material->metallicIsImage() ? -1.0f : material->getMetallic());
                            _geometryPipeline->setFloat("material.roughness", material->roughnessIsImage() ? -1.0f : material->getRoughness());
                            _geometryPipeline->setFloat("material.ao", material->aoIsImage() ? -1.0f : material->getAo());
                        } else {
                            resource::Material::CreateInfo defaultMaterial{};
                            _geometryPipeline->setVec3("material.albedo", defaultMaterial.color);
                            _geometryPipeline->setFloat("material.metallic", defaultMaterial.metallic);
                            _geometryPipeline->setFloat("material.roughness", defaultMaterial.roughness);
                            _geometryPipeline->setFloat("material.ao", defaultMaterial.ao);
                        }

                        // Draw mesh
                        _geometryPipeline->renderMesh(mesh->sid);
                    }
                }
            }
            _geometryPipeline->end();

            _gridPipeline->render(camera);

            if (_renderDrawer)
                _drawerPipeline->render(camera);
        }
        _geometryRenderPass->end();
    }
    _renderQueue->end();
    //_geometryPipeline->begin();
    //{
    //     //---------- PBR shader ----------//
    //     std::shared_ptr<Shader> shader = _geometryPipeline->getShader();
    //     shader->bind();

    //    shader->setMat4("projection", transpose(camera->getProj()));
    //    shader->setMat4("view", transpose(camera->getView()));
    //    shader->setVec3("camPos", camera->getPosition());
    //    shader->setMat4("directionalLightMatrix", transpose(_directionalLightMatrix));
    //    shader->setImage("directionalShadowMap", _shadowMapPipeline->getRenderPass()->getFramebuffer()->getImage());
    //    shader->setCubemap("omniShadowMap", _omnidirectionalShadowMap);
    //    shader->setFloat("omniFarPlane", 25.0f);

    //    // Always set environment textures (if there is no environment map, use white texture)
    //    shader->setCubemap("irradianceMap", "PbrRenderer::irradiance");
    //    shader->setCubemap("prefilterMap", "PbrRenderer::prefilter");
    //    shader->setImage("brdfLUT", "PbrRenderer::brdfLUT");

    //    if (_lastEnvironmentMap != "Not defined"_sid)
    //        shader->setInt("numEnvironmentLights", 1);
    //    else {
    //        LOG_WARN("graphics::PbrRenderer", "Number of environment light should always be 1 (white texture if not defined)");
    //        shader->setInt("numEnvironmentLights", 0);
    //    }
    //    shader->setMat3("environmentLightOri", mat3(1.0f));

    //    //----- Lighting -----//
    //    _environmentMapOri = mat3(1.0f);
    //    int numPointLights = 0;
    //    int numDirectionalLights = 0;
    //    for (auto entity : entities) {
    //        component::Transform* transform = component::getComponent<component::Transform>(entity);
    //        component::PointLight* pl = component::getComponent<component::PointLight>(entity);
    //        component::DirectionalLight* dl = component::getComponent<component::DirectionalLight>(entity);
    //        component::EnvironmentLight* el = component::getComponent<component::EnvironmentLight>(entity);

    //        if (transform && (pl || dl || el)) {
    //            if (pl && numPointLights < 10) {
    //                vec3 position = transform->getWorldTransformMatrix(entity).getPosition();
    //                int i = numPointLights++;
    //                shader->setVec3(("pointLights[" + std::to_string(i) + "].position").c_str(), position);
    //                shader->setVec3(("pointLights[" + std::to_string(i) + "].intensity").c_str(), pl->intensity);
    //            }
    //            if (dl) {
    //                vec3 base = {0.0f, 0.0f, -1.0f};
    //                mat4 rotation;
    //                rotation.setPosOri({0, 0, 0}, transform->orientation);
    //                base = transpose(mat3(rotation)) * base;
    //                shader->setVec3("directionalLight.direction", base);
    //                shader->setVec3("directionalLight.intensity", dl->intensity);
    //                numDirectionalLights++;
    //            }
    //            if (el) {
    //                mat4 ori;
    //                ori.setPosOri(vec3(0.0f), transform->orientation);
    //                _environmentMapOri = mat3(ori);
    //                shader->setMat3("environmentLightOri", transpose(_environmentMapOri));
    //            }
    //            if (numPointLights++ == 10)
    //                LOG_WARN("graphics::PbrRenderer", "Maximum number of point lights reached, 10 lights");
    //        }
    //    }
    //    shader->setInt("numPointLights", numPointLights);
    //    shader->setInt("numDirectionalLights", numDirectionalLights ? 1 : 0);

    //    //----- Entities -----//
    //    for (auto entity : entities) {
    //        component::Mesh* mesh = component::getComponent<component::Mesh>(entity);
    //        component::Transform* transform = component::getComponent<component::Transform>(entity);
    //        component::Material* compMat = component::getComponent<component::Material>(entity);
    //        resource::Material* material = compMat ? compMat->getResource() : nullptr;

    //        if (mesh && transform) {
    //            mat4 model = transpose(transform->getWorldTransformMatrix(entity));
    //            mat4 invModel = inverse(model);
    //            shader->setMat4("model", model);
    //            shader->setMat4("invModel", invModel);

    //            if (material) {
    //                if (material->colorIsImage()) {
    //                    shader->setImage("albedoTexture", material->colorImage);
    //                    shader->setVec3("material.albedo", {-1, -1, -1});
    //                } else
    //                    shader->setVec3("material.albedo", material->color);

    //                if (material->metallicIsImage()) {
    //                    shader->setImage("metallicTexture", material->metallicImage);
    //                    shader->setFloat("material.metallic", -1);
    //                } else
    //                    shader->setFloat("material.metallic", material->metallic);

    //                if (material->roughnessIsImage()) {
    //                    shader->setImage("roughnessTexture", material->roughnessImage);
    //                    shader->setFloat("material.roughness", -1);
    //                } else
    //                    shader->setFloat("material.roughness", material->roughness);

    //                if (material->aoIsImage()) {
    //                    shader->setImage("aoTexture", material->aoImage);
    //                    shader->setFloat("material.ao", -1);
    //                } else
    //                    shader->setFloat("material.ao", material->ao);

    //                if (material->hasNormalImage()) {
    //                    shader->setImage("normalTexture", material->normalImage);
    //                    shader->setInt("material.hasNormalTexture", 1);
    //                } else
    //                    shader->setInt("material.hasNormalTexture", 0);
    //            } else {
    //                resource::Material::CreateInfo defaultMaterial{};
    //                shader->setVec3("material.albedo", defaultMaterial.color);
    //                shader->setFloat("material.metallic", defaultMaterial.metallic);
    //                shader->setFloat("material.roughness", defaultMaterial.roughness);
    //                shader->setFloat("material.ao", defaultMaterial.ao);
    //                shader->setInt("material.hasNormalTexture", 0);
    //            }

    //            graphics::getGraphicsAPI()->renderMesh(mesh->sid);
    //        }
    //    }

    //    //---------- Background shader ----------//
    //    if (_lastEnvironmentMap != "Not defined"_sid) {
    //        _backgroundShader->bind();
    //        _backgroundShader->setMat4("projection", transpose(camera->getProj()));
    //        _backgroundShader->setMat4("view", transpose(camera->getView()));
    //        _backgroundShader->setCubemap("environmentMap", _lastEnvironmentMap);
    //        //_backgroundShader->setCubemap("environmentMap", _omnidirectionalShadowMap);
    //        //_backgroundShader->setCubemap("environmentMap", "PbrRenderer::irradiance");
    //        _backgroundShader->setMat3("environmentMapOri", _environmentMapOri);

    //        graphics::getGraphicsAPI()->renderCube();
    //    }
    //}
    //_geometryPipeline->end();
}

void PbrRenderer::irradianceCubemap() {
    //// Create shader
    // std::shared_ptr<Shader> shader = graphics::create<Shader>("shaders/compute/irradiance.asl");

    //// Generate irradiance cubemap
    // GraphicsAPI::GenerateProcessedCubemapInfo info;
    // info.cubemapSid = StringId("PbrRenderer::irradiance");
    // info.shader = shader;
    // info.width = 128;
    // info.height = 128;
    // info.numMipLevels = 1;
    // info.func = [&](std::shared_ptr<Shader> shader, mat4 proj, mat4 view, int face, int mipLevel) {
    //     if (mipLevel == 0 && face == 0) {
    //         // TODO shader->setCubemap("environmentMap", _lastEnvironmentMap);
    //         shader->setMat4("projection", transpose(proj));
    //     }
    //     shader->setMat4("view", transpose(view));
    // };
    // graphics::getGraphicsAPI()->generateProcessedCubemap(info);
}

void PbrRenderer::prefilterCubemap() {
    //// Create shader
    // std::shared_ptr<Shader> shader = graphics::create<Shader>("shaders/compute/asl.vert");

    //// Generate prefilter cubemap
    // GraphicsAPI::GenerateProcessedCubemapInfo info;
    // info.cubemapSid = StringId("PbrRenderer::prefilter");
    // info.shader = shader;
    // info.width = 512;
    // info.height = 512;
    // info.numMipLevels = 5;
    // info.func = [&](std::shared_ptr<Shader> shader, mat4 proj, mat4 view, int face, int mipLevel) {
    //     if (mipLevel == 0 && face == 0) {
    //         // TODO shader->setCubemap("environmentMap", _lastEnvironmentMap);
    //         shader->setMat4("projection", transpose(proj));
    //     }

    //    shader->setMat4("view", transpose(view));

    //    float roughness = (float)mipLevel / (float)(info.numMipLevels - 1);
    //    shader->setFloat("roughness", roughness);
    //};
    // graphics::getGraphicsAPI()->generateProcessedCubemap(info);
}

void PbrRenderer::brdfLUT() {
    // std::shared_ptr<Shader> shader = graphics::create<Shader>("shaders/compute/brdf.asl");

    //// Generate brdf LUT
    // GraphicsAPI::GenerateProcessedTextureInfo info;
    // info.textureSid = StringId("PbrRenderer::brdfLUT");
    // info.shader = shader;
    // info.imageInfo.format = Image::Format::RG16F;
    // info.imageInfo.samplerWrap = Image::Wrap::REPEAT;
    // info.imageInfo.width = 512;
    // info.imageInfo.height = 512;
    // info.imageInfo.mipLevels = 1;
    // info.imageInfo.debugName = StringId("PbrRenderer brdfLUT image");

    // graphics::getGraphicsAPI()->generateProcessedTexture(info);
}

} // namespace atta::graphics
