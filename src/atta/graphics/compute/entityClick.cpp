//--------------------------------------------------
// Atta Graphics Module
// entityClick.cpp
// Date: 2021-11-14
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/graphics/compute/entityClick.h>

#include <atta/graphics/framebuffer.h>
#include <atta/graphics/interface.h>
#include <atta/graphics/renderPass.h>
#include <atta/component/components/mesh.h>
#include <atta/component/components/transform.h>
#include <atta/component/interface.h>
#include <atta/graphics/apis/openGL/openGL.h>

namespace atta::graphics {

EntityClick::EntityClick() : _width(500), _height(500) {
    // Framebuffer
    Framebuffer::CreateInfo framebufferInfo{};
    framebufferInfo.attachments.push_back({Image::Format::RED32I});
    framebufferInfo.attachments.push_back({Image::Format::DEPTH32F});
    framebufferInfo.width = _width;
    framebufferInfo.height = _height;
    framebufferInfo.debugName = StringId("EntityClick Framebuffer");
    std::shared_ptr<Framebuffer> framebuffer = graphics::create<Framebuffer>(framebufferInfo);

    // Render Pass
    RenderPass::CreateInfo renderPassInfo{};
    renderPassInfo.framebuffer = framebuffer;
    renderPassInfo.debugName = StringId("EntityClick Render Pass");
    std::shared_ptr<RenderPass> renderPass = graphics::create<RenderPass>(renderPassInfo);

    //---------- Graphics pipeline ----------//
    // Shader
    std::shared_ptr<Shader> shader = graphics::create<Shader>("shaders/compute/entityClick.asl");

    Pipeline::CreateInfo pipelineInfo{};
    // Vertex input layout
    pipelineInfo.shader = shader;
    pipelineInfo.renderPass = renderPass;
    _geometryPipeline = graphics::create<Pipeline>(pipelineInfo);
}

component::EntityId EntityClick::click(std::shared_ptr<Viewport> viewport, vec2i pos) {
    component::EntityId eid;
    component::EntityId maxEid = -1;
    unsigned width = viewport->getWidth();
    unsigned height = viewport->getHeight();
    // Resize
    if (width != _width || height != _height) {
        _geometryPipeline->getRenderPass()->getFramebuffer()->resize(width, height);
        _width = width;
        _height = height;
    }

    // Render entity ids
    std::vector<component::EntityId> entities = component::getNoPrototypeView();

    _geometryPipeline->begin();
    {
        std::shared_ptr<Shader> shader = _geometryPipeline->getShader();

        mat4 m(1.0f);
        glDisable(GL_DEPTH_TEST);
        shader->setMat4("model", m);
        shader->setMat4("projection", m);
        shader->setMat4("view", m);
        shader->setInt("entityId", -1);
        graphics::getGraphicsAPI()->renderQuad3();
        glEnable(GL_DEPTH_TEST);
        glClear(GL_DEPTH_BUFFER_BIT); // XXX Not sure why but it only works in the browser if clear the depth buffer

        // Render entities
        shader->setMat4("projection", transpose(viewport->getCamera()->getProj()));
        shader->setMat4("view", transpose(viewport->getCamera()->getView()));

        for (auto entity : entities) {
            component::Mesh* mesh = component::getComponent<component::Mesh>(entity);
            component::Transform* transform = component::getComponent<component::Transform>(entity);

            if (mesh && transform) {
                mat4 model = transpose(transform->getWorldTransformMatrix(entity));
                shader->setMat4("model", model);

                // component::EntityId
                shader->setInt("entityId", entity);
                maxEid = std::max((int)maxEid, entity);

                // Draw mesh
                graphics::getGraphicsAPI()->renderMesh(mesh->sid);
            }
        }

        // Get pixel id
        eid = _geometryPipeline->getRenderPass()->getFramebuffer()->readPixel(0, pos.x, pos.y);
    }
    _geometryPipeline->end();

    return eid > maxEid ? -1 : eid;
}

} // namespace atta::graphics
