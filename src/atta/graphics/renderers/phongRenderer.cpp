//--------------------------------------------------
// Atta Graphics Module
// phongRenderer.cpp
// Date: 2021-09-18
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/graphics/renderers/phongRenderer.h>

#include <atta/graphics/framebuffer.h>
#include <atta/graphics/manager.h>
#include <atta/graphics/renderPass.h>
#include <atta/graphics/rendererAPIs/openGL/openGLShaderGroup.h>

#include <atta/resource/manager.h>
#include <atta/resource/resources/mesh.h>

#include <atta/component/components/directionalLight.h>
#include <atta/component/components/material.h>
#include <atta/component/components/mesh.h>
#include <atta/component/components/pointLight.h>
#include <atta/component/components/transform.h>
#include <atta/component/factory.h>
#include <atta/component/interface.h>

namespace atta::graphics {

PhongRenderer::PhongRenderer() : Renderer("PhongRenderer") {
    //---------- Create geometry pipeline ----------//
    // Framebuffer
    Framebuffer::CreateInfo framebufferInfo{};
    framebufferInfo.attachments.push_back({Image::Format::RGB});
    framebufferInfo.attachments.push_back({Image::Format::DEPTH24_STENCIL8});
    framebufferInfo.width = 500;
    framebufferInfo.height = 500;
    framebufferInfo.debugName = StringId("PhongRenderer Framebuffer");
    std::shared_ptr<Framebuffer> framebuffer = Manager::create<Framebuffer>(framebufferInfo);

    // Shader Group
    ShaderGroup::CreateInfo shaderGroupInfo{};
    shaderGroupInfo.shaderPaths = {"shaders/phongRenderer/shader.vert", "shaders/phongRenderer/shader.frag"};
    shaderGroupInfo.debugName = StringId("PhongRenderer Shader Group");
    std::shared_ptr<ShaderGroup> shaderGroup = Manager::create<ShaderGroup>(shaderGroupInfo);

    // Render Pass
    RenderPass::CreateInfo renderPassInfo{};
    renderPassInfo.framebuffer = framebuffer;
    renderPassInfo.debugName = StringId("PhongRenderer Render Pass");
    std::shared_ptr<RenderPass> renderPass = Manager::create<RenderPass>(renderPassInfo);

    Pipeline::CreateInfo pipelineInfo{};
    // Vertex input layout
    pipelineInfo.shaderGroup = shaderGroup;
    pipelineInfo.layout = {{"inPosition", VertexBufferElement::Type::VEC3},
                           {"inNormal", VertexBufferElement::Type::VEC3},
                           {"inTexCoord", VertexBufferElement::Type::VEC2}};
    pipelineInfo.renderPass = renderPass;
    _geometryPipeline = Manager::create<Pipeline>(pipelineInfo);

    //---------- Common pipelines ----------//
    _selectedPipeline = std::make_unique<SelectedPipeline>(renderPass, pipelineInfo.layout);
    _drawerPipeline = std::make_unique<DrawerPipeline>(renderPass);
}

PhongRenderer::~PhongRenderer() {}

void PhongRenderer::render(std::shared_ptr<Camera> camera) {
    std::vector<component::EntityId> entities = component::getNoPrototypeView();
    _geometryPipeline->begin();
    {
        std::shared_ptr<ShaderGroup> shader = _geometryPipeline->getShaderGroup();

        shader->setMat4("projection", transpose(camera->getProj()));
        shader->setMat4("view", transpose(camera->getView()));
        shader->setVec3("viewPos", camera->getPosition());

        //----- Lighting -----//
        int numPointLights = 0;
        for (auto entity : entities) {
            component::Transform* transform = component::getEntityComponent<component::Transform>(entity);
            component::PointLight* pl = component::getEntityComponent<component::PointLight>(entity);
            component::DirectionalLight* dl = component::getEntityComponent<component::DirectionalLight>(entity);

            if (transform && (pl || dl)) {
                if (pl && numPointLights < 10) {
                    vec3 position = transform->getWorldTransform(entity).getPosition();
                    int i = numPointLights++;
                    shader->setVec3(("pointLights[" + std::to_string(i) + "].position").c_str(), position);
                    shader->setVec3(("pointLights[" + std::to_string(i) + "].intensity").c_str(), pl->intensity);
                }
                if (dl) {
                    vec3 before = {0.0f, -1.0f, 0.0f};
                    // vec3 direction;
                    // transform->orientation.transformVector(before, direction);
                    shader->setVec3("directionalLight.direction", before);
                    shader->setVec3("directionalLight.intensity", dl->intensity);
                }
                if (numPointLights++ == 10)
                    LOG_WARN("graphics::PhongRenderer", "Maximum number of point lights reached, 10 lights");
            }
        }
        shader->setInt("numPointLights", numPointLights);

        // Ambient
        shader->setVec3("ambientColor", {0.3f, 0.3f, 0.3f});
        shader->setFloat("ambientStrength", 1.0f);
        // Diffuse
        shader->setFloat("diffuseStrength", 1.0f);
        // Specular
        shader->setFloat("specularStrength", 0.5f);

        for (auto entity : entities) {
            component::Mesh* mesh = component::getEntityComponent<component::Mesh>(entity);
            component::Transform* transform = component::getEntityComponent<component::Transform>(entity);
            component::Material* material = component::getEntityComponent<component::Material>(entity);

            if (mesh && transform) {
                mat4 model = transpose(transform->getWorldTransform(entity));
                mat4 invModel = inverse(model);
                shader->setMat4("model", model);
                shader->setMat4("invModel", invModel);

                if (material) {
                    if (material->albedoTexture.getId() != SID("Empty texture")) {
                        shader->setTexture("albedoTexture", material->albedoTexture);
                        shader->setVec3("material.albedo", {-1, -1, -1});
                    } else
                        shader->setVec3("material.albedo", material->albedo);

                    if (material->metallicTexture.getId() != SID("Empty texture")) {
                        shader->setTexture("metallicTexture", material->metallicTexture);
                        shader->setFloat("material.metallic", -1);
                    } else
                        shader->setFloat("material.metallic", material->metallic);

                    if (material->roughnessTexture.getId() != SID("Empty texture")) {
                        shader->setTexture("roughnessTexture", material->roughnessTexture);
                        shader->setFloat("material.roughness", -1);
                    } else
                        shader->setFloat("material.roughness", material->roughness);

                    if (material->aoTexture.getId() != SID("Empty texture")) {
                        shader->setTexture("aoTexture", material->aoTexture);
                        shader->setFloat("material.ao", -1);
                    } else
                        shader->setFloat("material.ao", material->ao);
                } else {
                    component::Material material{};
                    shader->setVec3("material.albedo", material.albedo);
                    shader->setFloat("material.metallic", material.metallic);
                    shader->setFloat("material.roughness", material.roughness);
                    shader->setFloat("material.ao", material.ao);
                }

                Manager::getRendererAPI()->renderMesh(mesh->sid);
            }
        }
    }
    _geometryPipeline->end();

    if (_renderSelected)
        _selectedPipeline->render(camera);
    if (_renderDrawer)
        _drawerPipeline->render(camera);
}

void PhongRenderer::resize(uint32_t width, uint32_t height) {
    if (width != _width || height != _height) {
        _geometryPipeline->getRenderPass()->getFramebuffer()->resize(width, height);
        _width = width;
        _height = height;
    }
}

} // namespace atta::graphics
