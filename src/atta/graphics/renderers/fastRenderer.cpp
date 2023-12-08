//--------------------------------------------------
// Atta Graphics Module
// fastRenderer.cpp
// Date: 2021-09-10
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/graphics/renderers/fastRenderer.h>

#include <atta/graphics/framebuffer.h>
#include <atta/graphics/interface.h>
#include <atta/graphics/renderPass.h>

#include <atta/resource/interface.h>
#include <atta/resource/resources/mesh.h>

#include <atta/component/components/material.h>
#include <atta/component/components/mesh.h>
#include <atta/component/components/transform.h>
#include <atta/component/factory.h>
#include <atta/component/interface.h>

namespace atta::graphics {

FastRenderer::FastRenderer() : Renderer("FastRenderer") {
    //---------- Create geometry pipeline ----------//
    // Framebuffer
    Framebuffer::CreateInfo framebufferInfo{};
    framebufferInfo.attachments.push_back({Image::Format::RGBA});
    framebufferInfo.attachments.push_back({Image::Format::DEPTH24_STENCIL8});
    framebufferInfo.width = 500;
    framebufferInfo.height = 500;
    framebufferInfo.debugName = StringId("FastRenderer Framebuffer");
    std::shared_ptr<Framebuffer> framebuffer = graphics::create<Framebuffer>(framebufferInfo);

    // Shader
    std::shared_ptr<Shader> shader = graphics::create<Shader>("shaders/fastRenderer/fastRenderer.asl");

    // Render Pass
    RenderPass::CreateInfo renderPassInfo{};
    renderPassInfo.framebuffer = framebuffer;
    renderPassInfo.debugName = StringId("FastRenderer Render Pass");
    std::shared_ptr<RenderPass> renderPass = graphics::create<RenderPass>(renderPassInfo);

    // Pipeline
    Pipeline::CreateInfo pipelineInfo{};
    pipelineInfo.shader = shader;
    pipelineInfo.renderPass = renderPass;
    _geometryPipeline = graphics::create<Pipeline>(pipelineInfo);

    //---------- Common pipelines ----------//
    _selectedPipeline = std::make_unique<SelectedPipeline>(renderPass);
    _drawerPipeline = std::make_unique<DrawerPipeline>(renderPass);
}

FastRenderer::~FastRenderer() {}

void FastRenderer::render(std::shared_ptr<Camera> camera) {
    std::vector<component::EntityId> entities = component::getNoPrototypeView();
    _geometryPipeline->begin();
    {
        std::shared_ptr<Shader> shader = _geometryPipeline->getShader();

        shader->setMat4("projection", transpose(camera->getProj()));
        shader->setMat4("view", transpose(camera->getView()));

        for (auto entity : entities) {
            component::Mesh* mesh = component::getComponent<component::Mesh>(entity);
            component::Transform* transform = component::getComponent<component::Transform>(entity);
            component::Material* compMat = component::getComponent<component::Material>(entity);
            resource::Material* material = compMat ? compMat->getResource() : nullptr;

            if (mesh && transform) {
                mat4 model = transpose(transform->getWorldTransformMatrix(entity));
                shader->setMat4("model", model);

                if (material) {
                    if (material->colorIsImage()) {
                        shader->setImage("albedoTexture", material->colorImage);
                        shader->setVec3("albedo", {-1, -1, -1});
                    } else
                        shader->setVec3("albedo", material->color);
                } else {
                    resource::Material::CreateInfo defaultMaterial {};
                    shader->setVec3("albedo", defaultMaterial.color);
                }

                // Draw mesh
                graphics::getGraphicsAPI()->renderMesh(mesh->sid);
            }
        }
    }
    _geometryPipeline->end();

    if (_renderSelected)
        _selectedPipeline->render(camera);
    if (_renderDrawer)
        _drawerPipeline->render(camera);
}

void FastRenderer::resize(uint32_t width, uint32_t height) {
    if (width != _width || height != _height) {
        _geometryPipeline->getRenderPass()->getFramebuffer()->resize(width, height);
        _width = width;
        _height = height;
    }
}

} // namespace atta::graphics
