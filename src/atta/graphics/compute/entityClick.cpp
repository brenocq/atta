//--------------------------------------------------
// Atta Graphics Module
// entityClick.cpp
// Date: 2021-11-14
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/graphics/compute/entityClick.h>

#include <atta/component/components/mesh.h>
#include <atta/component/components/transform.h>
#include <atta/component/interface.h>
#include <atta/graphics/apis/openGL/openGL.h>
#include <atta/graphics/framebuffer.h>
#include <atta/graphics/interface.h>
#include <atta/graphics/renderPass.h>

namespace atta::graphics {

EntityClick::EntityClick() : _width(500), _height(500) {
    // Render Queue
    _renderQueue = graphics::create<RenderQueue>();

    // Framebuffer
    Framebuffer::CreateInfo framebufferInfo{};
    framebufferInfo.attachments.push_back({Image::Format::RED32I});
    framebufferInfo.attachments.push_back({Image::Format::DEPTH32F});
    framebufferInfo.width = _width;
    framebufferInfo.height = _height;
    framebufferInfo.clearColor = {0, 0, 0, 0};
    framebufferInfo.debugName = StringId("EntityClick Framebuffer");
    std::shared_ptr<Framebuffer> framebuffer = graphics::create<Framebuffer>(framebufferInfo);

    // Render Pass
    RenderPass::CreateInfo renderPassInfo{};
    renderPassInfo.framebuffer = framebuffer;
    renderPassInfo.debugName = StringId("EntityClick Render Pass");
    _renderPass = graphics::create<RenderPass>(renderPassInfo);

    //---------- Graphics pipeline ----------//
    // Vertex input layout
    Pipeline::CreateInfo pipelineInfo{};
    pipelineInfo.shader = graphics::create<Shader>("shaders/compute/entityClick.asl");
    pipelineInfo.renderPass = _renderPass;
    renderPassInfo.debugName = StringId("EntityClick Pipeline");
    _geometryPipeline = graphics::create<Pipeline>(pipelineInfo);
}

component::EntityId EntityClick::click(std::shared_ptr<Renderer> renderer, std::shared_ptr<Camera> camera, vec2i pos) {
    component::EntityId eid = -1;
    component::EntityId maxEid = -1;
    unsigned width = renderer->getWidth();
    unsigned height = renderer->getHeight();
    // Resize
    if (width != _width || height != _height) {
        _geometryPipeline->resize(width, height);
        _width = width;
        _height = height;
    }

    // Render entity ids
    std::vector<component::EntityId> entities = component::getNoPrototypeView();

    // Render
    _renderQueue->begin();
    {
        _renderPass->begin(_renderQueue);
        {
            _geometryPipeline->begin();
            {
                // Render entities with eid
                std::vector<component::EntityId> entities = component::getNoPrototypeView();
                _geometryPipeline->setMat4("projection", camera->getProj());
                _geometryPipeline->setMat4("view", camera->getView());

                for (auto entity : entities) {
                    component::Mesh* mesh = component::getComponent<component::Mesh>(entity);
                    component::Transform* transform = component::getComponent<component::Transform>(entity);

                    if (mesh && transform) {
                        mat4 model = transform->getWorldTransformMatrix(entity);
                        _geometryPipeline->setMat4("model", model);

                        // component::EntityId
                        _geometryPipeline->setInt("entityId", int(entity) + 1);
                        maxEid = std::max(maxEid, entity);

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

    // Get entityId at pixel
    std::vector<uint8_t> pixel = _renderPass->getFramebuffer()->getImage(0)->read(pos, vec2i(1, 1));
    if (pixel.size() == sizeof(int)) {
        int* eid = (int*)pixel.data();
        *eid -= 1;
        if (*eid <= maxEid && *eid >= 0)
            return component::EntityId(*eid);
    }

    return -1;
}

} // namespace atta::graphics
