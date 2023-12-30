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
    _width = framebufferInfo.width = 500;
    _height = framebufferInfo.height = 500;
    framebufferInfo.clearColor = {0.2f, 0.2f, 0.2f, 1.0f};
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
            _geometryPipeline->begin();
            {
                std::vector<component::EntityId> entities = component::getNoPrototypeView();
                _geometryPipeline->setMat4("uProjection", camera->getProj());
                _geometryPipeline->setMat4("uView", camera->getView());
                _geometryPipeline->setVec3("uViewPos", camera->getPosition());
                _geometryPipeline->setVec3("uAmbientColor", {0.3f, 0.3f, 0.3f});
                _geometryPipeline->setFloat("uAmbientStrength", 1.0f);
                _geometryPipeline->setFloat("uDiffuseStrength", 1.0f);
                _geometryPipeline->setFloat("uSpecularStrength", 0.5f);

                // Process lights
                //----- Lighting -----//
                int numPointLights = 0;
                bool hasDirectionalLight = false;
                for (auto entity : entities) {
                    component::Transform* transform = component::getComponent<component::Transform>(entity);
                    component::PointLight* pl = component::getComponent<component::PointLight>(entity);
                    component::DirectionalLight* dl = component::getComponent<component::DirectionalLight>(entity);

                    if (transform && (pl || dl)) {
                        if (pl && numPointLights < 10) {
                            vec3 position = transform->getWorldTransformMatrix(entity).getPosition();
                            int i = numPointLights++;
                            _geometryPipeline->setVec3(("uPointLights[" + std::to_string(i) + "].position").c_str(), position);
                            _geometryPipeline->setVec3(("uPointLights[" + std::to_string(i) + "].intensity").c_str(), pl->intensity);
                        }
                        if (dl) {
                            hasDirectionalLight = true;
                            vec3 direction = {0.0f, 0.0f, -1.0f};
                            transform->orientation.rotateVector(direction);
                            _geometryPipeline->setVec3("uDirectionalLight.direction", direction);
                            _geometryPipeline->setVec3("uDirectionalLight.intensity", dl->intensity);
                        }
                        if (numPointLights++ == 10)
                            LOG_WARN("gfx::PhongRenderer", "Maximum number of point lights reached, 10 lights");
                    }
                }
                _geometryPipeline->setInt("uNumPointLights", numPointLights);
                _geometryPipeline->setBool("uHasDirectionalLight", hasDirectionalLight);

                //----- Meshes -----//
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
                            _geometryPipeline->setVec3("uMaterial.albedo", material->colorIsImage() ? vec3(-1, -1, -1) : material->getColor());
                            _geometryPipeline->setFloat("uMaterial.metallic", material->metallicIsImage() ? -1.0f : material->getMetallic());
                            _geometryPipeline->setFloat("uMaterial.roughness", material->roughnessIsImage() ? -1.0f : material->getRoughness());
                            _geometryPipeline->setFloat("uMaterial.ao", material->aoIsImage() ? -1.0f : material->getAo());
                        } else {
                            resource::Material::CreateInfo defaultMaterial{};
                            _geometryPipeline->setVec3("uMaterial.albedo", defaultMaterial.color);
                            _geometryPipeline->setFloat("uMaterial.metallic", defaultMaterial.metallic);
                            _geometryPipeline->setFloat("uMaterial.roughness", defaultMaterial.roughness);
                            _geometryPipeline->setFloat("uMaterial.ao", defaultMaterial.ao);
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
