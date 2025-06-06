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

#include <atta/component/components/material.h>
#include <atta/component/components/mesh.h>
#include <atta/component/components/transform.h>
#include <atta/component/factory.h>
#include <atta/component/interface.h>

namespace atta::graphics {

FastRenderer::FastRenderer() : Renderer("FastRenderer"), _wasResized(false) {
    // Render Queue
    _renderQueue = graphics::create<RenderQueue>();

    // Framebuffer
    Framebuffer::CreateInfo framebufferInfo{};
    framebufferInfo.attachments.push_back({Image::Format::RGBA});
    framebufferInfo.attachments.push_back({Image::Format::DEPTH24_STENCIL8});
    _width = framebufferInfo.width = 500;
    _height = framebufferInfo.height = 500;
    framebufferInfo.clearColor = {0.2f, 0.2f, 0.2f, 1.0f};
    framebufferInfo.debugName = StringId("FastRenderer Framebuffer");

    // Render Pass
    RenderPass::CreateInfo renderPassInfo{};
    renderPassInfo.framebuffer = graphics::create<Framebuffer>(framebufferInfo);
    renderPassInfo.debugName = StringId("FastRenderer RenderPass");
    _renderPass = graphics::create<RenderPass>(renderPassInfo);

    //---------- Create geometry pipeline ----------//
    Pipeline::CreateInfo pipelineInfo{};
    pipelineInfo.shader = graphics::create<Shader>("shaders/fastRenderer/fastRenderer.asl");
    pipelineInfo.renderPass = _renderPass;
    pipelineInfo.debugName = StringId("FastRenderer Pipeline");
    _geometryPipeline = graphics::create<Pipeline>(pipelineInfo);

    //---------- Common pipelines ----------//
    _drawerPipeline = std::make_unique<DrawerPipeline>(_renderPass);
    _gridPipeline = std::make_unique<GridPipeline>(_renderPass);
    _selectedPipeline = std::make_unique<SelectedPipeline>(_renderPass);
}

FastRenderer::~FastRenderer() {}

void FastRenderer::render(std::shared_ptr<Camera> camera) {
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
        _renderPass->begin(_renderQueue);
        {
            _geometryPipeline->begin();
            {
                std::vector<component::EntityId> entities = component::getNoPrototypeView();
                _geometryPipeline->setMat4("uProjection", camera->getProj());
                _geometryPipeline->setMat4("uView", camera->getView());

                for (auto entity : entities) {
                    component::Mesh* mesh = component::getComponent<component::Mesh>(entity);
                    component::Transform* transform = component::getComponent<component::Transform>(entity);
                    component::Material* compMat = component::getComponent<component::Material>(entity);
                    resource::Material* material = compMat ? compMat->getResource() : nullptr;

                    if (mesh && transform) {
                        mat4 model = transform->getWorldTransformMatrix(entity);
                        _geometryPipeline->setMat4("uModel", model);

                        if (material) {
                            _geometryPipeline->setImageGroup(compMat->sid);
                            _geometryPipeline->setVec3("uAlbedo", material->colorIsImage() ? vec3(-1, -1, -1) : material->getColor());
                        } else {
                            resource::Material::CreateInfo defaultMaterial{};
                            _geometryPipeline->setVec3("uAlbedo", defaultMaterial.color);
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
        _renderPass->end();
    }
    _renderQueue->end();

    //// if (_renderSelected)
    ////     _selectedPipeline->render(camera);
}

void FastRenderer::resize(uint32_t width, uint32_t height) {
    if (width != _width || height != _height) {
        _wasResized = true;
        _width = width;
        _height = height;
    }
}

} // namespace atta::graphics
