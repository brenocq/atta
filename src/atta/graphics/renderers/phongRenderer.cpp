//--------------------------------------------------
// Atta Graphics Module
// phongRenderer.cpp
// Date: 2021-09-18
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/graphics/renderers/phongRenderer.h>

#include <atta/graphics/framebuffer.h>
#include <atta/graphics/interface.h>

#include <atta/resource/interface.h>
#include <atta/resource/resources/mesh.h>

#include <atta/component/components/directionalLight.h>
#include <atta/component/components/material.h>
#include <atta/component/components/mesh.h>
#include <atta/component/components/pointLight.h>
#include <atta/component/components/transform.h>
#include <atta/component/factory.h>
#include <atta/component/interface.h>

namespace atta::graphics {

PhongRenderer::PhongRenderer() : Renderer("PhongRenderer"), _wasResized(false) {
    // Render Queue
    _renderQueue = graphics::create<RenderQueue>();

    // Framebuffer
    Framebuffer::CreateInfo framebufferInfo{};
    framebufferInfo.attachments.push_back({Image::Format::RGBA});
    framebufferInfo.attachments.push_back({Image::Format::DEPTH24_STENCIL8});
    framebufferInfo.width = 500;
    framebufferInfo.height = 500;
    framebufferInfo.debugName = StringId("PhongRenderer Framebuffer");

    // Render Pass
    RenderPass::CreateInfo renderPassInfo{};
    renderPassInfo.framebuffer = graphics::create<Framebuffer>(framebufferInfo);
    renderPassInfo.debugName = StringId("PhongRenderer Render Pass");
    _renderPass = graphics::create<RenderPass>(renderPassInfo);

    //---------- Create geometry pipeline ----------//
    Pipeline::CreateInfo pipelineInfo{};
    pipelineInfo.shader = graphics::create<Shader>("shaders/phongRenderer/phongRenderer.asl");
    pipelineInfo.renderPass = _renderPass;
    _geometryPipeline = graphics::create<Pipeline>(pipelineInfo);

    //---------- Common pipelines ----------//
    _selectedPipeline = std::make_unique<SelectedPipeline>(_renderPass);
    _drawerPipeline = std::make_unique<DrawerPipeline>(_renderPass);
}

PhongRenderer::~PhongRenderer() {}

void PhongRenderer::render(std::shared_ptr<Camera> camera) {
    // Handle resize (ensure that last framebuffer command finished)
    if (_wasResized) {
        _geometryPipeline->resize(_width, _height);
        _wasResized = false;
    }

    // Handle image group update from materials
    _geometryPipeline->updateImageGroupsFromMaterials([](resource::Material* material) {
        Pipeline::ImageGroup imageGroup;
        if (material->colorIsImage())
            imageGroup.emplace_back("uAlbedoTexture", material->getColorImage());
        if (material->metallicIsImage())
            imageGroup.emplace_back("uMetallicTexture", material->getMetallicImage());
        if (material->roughnessIsImage())
            imageGroup.emplace_back("uRoughnessTexture", material->getRoughnessImage());
        if (material->aoIsImage())
            imageGroup.emplace_back("uAoTexture", material->getAoImage());
        return imageGroup;
    });

    // Render
    _renderQueue->begin();
    {
        _renderPass->begin(_renderQueue);
        {
            _geometryPipeline->begin(_renderQueue);
            {
                std::vector<component::EntityId> entities = component::getNoPrototypeView();
                _geometryPipeline->setMat4("uProjection", camera->getProj());
                _geometryPipeline->setMat4("uView", camera->getView());
                _geometryPipeline->setVec3("uViewPos", camera->getPosition());
                _geometryPipeline->setVec3("uAmbientColor", {0.3f, 0.9f, 0.3f});
                _geometryPipeline->setFloat("uAmbientStrength", 1.0f);
                _geometryPipeline->setFloat("uDiffuseStrength", 1.0f);
                _geometryPipeline->setFloat("uSpecularStrength", 0.5f);
                _geometryPipeline->setInt("uNumPointLights", 0);

                for (auto entity : entities) {
                    component::Mesh* mesh = component::getComponent<component::Mesh>(entity);
                    component::Transform* transform = component::getComponent<component::Transform>(entity);
                    component::Material* compMat = component::getComponent<component::Material>(entity);
                    resource::Material* material = compMat ? compMat->getResource() : nullptr;

                    if (mesh && transform) {
                        mat4 model = transform->getWorldTransformMatrix(entity);
                        _geometryPipeline->setMat4("uModel", model);
                        _geometryPipeline->setMat4("uInvModel", inverse(model));

                        if (material) {
                            _geometryPipeline->setImageGroup(compMat->sid);

                            if (material->colorIsImage())
                                _geometryPipeline->setVec3("uAlbedo", {-1, -1, -1});
                            else
                                _geometryPipeline->setVec3("uAlbedo", material->getColor());

                            if (material->metallicIsImage())
                                _geometryPipeline->setFloat("uMetallic", -1);
                            else
                                _geometryPipeline->setFloat("uMetallic", material->getMetallic());

                            if (material->roughnessIsImage())
                                _geometryPipeline->setFloat("uRoughness", -1);
                            else
                                _geometryPipeline->setFloat("uRoughness", material->getRoughness());

                            if (material->aoIsImage())
                                _geometryPipeline->setFloat("uAo", -1);
                            else
                                _geometryPipeline->setFloat("uAo", material->getAo());
                        } else {
                            resource::Material::CreateInfo defaultMaterial{};
                            _geometryPipeline->setVec3("uAlbedo", defaultMaterial.color);
                            _geometryPipeline->setFloat("uMetallic", defaultMaterial.metallic);
                            _geometryPipeline->setFloat("uRoughness", defaultMaterial.roughness);
                            _geometryPipeline->setFloat("uAo", defaultMaterial.ao);
                        }

                        // Draw mesh
                        _geometryPipeline->renderMesh(mesh->sid);
                    }
                }
            }
            _geometryPipeline->end();
        }
        _renderPass->end();
    }
    _renderQueue->end();

    // graphics::beginRender();

    // std::vector<component::EntityId> entities = component::getNoPrototypeView();
    //_geometryPipeline->begin();
    //{
    //     std::shared_ptr<Shader> shader = _geometryPipeline->getShader();

    //    shader->setMat4("projection", transpose(camera->getProj()));
    //    shader->setMat4("view", transpose(camera->getView()));
    //    shader->setVec3("viewPos", camera->getPosition());

    //    //----- Lighting -----//
    //    int numPointLights = 0;
    //    for (auto entity : entities) {
    //        component::Transform* transform = component::getComponent<component::Transform>(entity);
    //        component::PointLight* pl = component::getComponent<component::PointLight>(entity);
    //        component::DirectionalLight* dl = component::getComponent<component::DirectionalLight>(entity);

    //        if (transform && (pl || dl)) {
    //            if (pl && numPointLights < 10) {
    //                vec3 position = transform->getWorldTransformMatrix(entity).getPosition();
    //                int i = numPointLights++;
    //                shader->setVec3(("pointLights[" + std::to_string(i) + "].position").c_str(), position);
    //                shader->setVec3(("pointLights[" + std::to_string(i) + "].intensity").c_str(), pl->intensity);
    //            }
    //            if (dl) {
    //                vec3 before = {0.0f, -1.0f, 0.0f};
    //                // vec3 direction;
    //                // transform->orientation.transformVector(before, direction);
    //                shader->setVec3("directionalLight.direction", before);
    //                shader->setVec3("directionalLight.intensity", dl->intensity);
    //            }
    //            if (numPointLights++ == 10)
    //                LOG_WARN("graphics::PhongRenderer", "Maximum number of point lights reached, 10 lights");
    //        }
    //    }
    //    shader->setInt("numPointLights", numPointLights);

    //    // Ambient
    //    shader->setVec3("ambientColor", {0.3f, 0.3f, 0.3f});
    //    shader->setFloat("ambientStrength", 1.0f);
    //    // Diffuse
    //    shader->setFloat("diffuseStrength", 1.0f);
    //    // Specular
    //    shader->setFloat("specularStrength", 0.5f);

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
    //            } else {
    //                resource::Material::CreateInfo material{};
    //                shader->setVec3("material.albedo", material.color);
    //                shader->setFloat("material.metallic", material.metallic);
    //                shader->setFloat("material.roughness", material.roughness);
    //                shader->setFloat("material.ao", material.ao);
    //            }

    //            graphics::getGraphicsAPI()->renderMesh(mesh->sid);
    //        }
    //    }
    //}
    //_geometryPipeline->end();

    // if (_renderSelected)
    //     _selectedPipeline->render(camera);
    // if (_renderDrawer)
    //     _drawerPipeline->render(camera);
}

void PhongRenderer::resize(uint32_t width, uint32_t height) {
    if (width != _width || height != _height) {
        _wasResized = true;
        _width = width;
        _height = height;
    }
}

} // namespace atta::graphics
