//--------------------------------------------------
// Atta Graphics Module
// entityClick.cpp
// Date: 2021-11-14
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/graphics/compute/entityClick.h>

#include <atta/graphics/framebuffer.h>
#include <atta/graphics/manager.h>
#include <atta/graphics/renderPass.h>
#include <atta/graphics/rendererAPIs/openGL/openGLShaderGroup.h>

#include <atta/component/components/mesh.h>
#include <atta/component/components/transform.h>
#include <atta/component/manager.h>

namespace atta::graphics {

EntityClick::EntityClick() : _width(500), _height(500) {
    // Framebuffer
    Framebuffer::CreateInfo framebufferInfo{};
    framebufferInfo.attachments.push_back({Image::Format::RED32I});
    framebufferInfo.attachments.push_back({Image::Format::DEPTH32F});
    framebufferInfo.width = _width;
    framebufferInfo.height = _height;
    framebufferInfo.debugName = StringId("EntityClick Framebuffer");
    std::shared_ptr<Framebuffer> framebuffer = Manager::create<Framebuffer>(framebufferInfo);

    // Render Pass
    RenderPass::CreateInfo renderPassInfo{};
    renderPassInfo.framebuffer = framebuffer;
    renderPassInfo.debugName = StringId("EntityClick Render Pass");
    std::shared_ptr<RenderPass> renderPass = Manager::create<RenderPass>(renderPassInfo);

    //---------- Graphics pipeline ----------//
    // Shader Group
    ShaderGroup::CreateInfo shaderGroupInfo{};
    shaderGroupInfo.shaderPaths = {"shaders/compute/entityClick.vert", "shaders/compute/entityClick.frag"};
    shaderGroupInfo.debugName = StringId("EntityClick Shader Group");
    std::shared_ptr<ShaderGroup> shaderGroup = Manager::create<ShaderGroup>(shaderGroupInfo);

    Pipeline::CreateInfo pipelineInfo{};
    // Vertex input layout
    pipelineInfo.shaderGroup = shaderGroup;
    pipelineInfo.layout = {{"inPosition", VertexBufferElement::Type::VEC3},
                           {"inNormal", VertexBufferElement::Type::VEC3},
                           {"inTexCoord", VertexBufferElement::Type::VEC2}};
    pipelineInfo.renderPass = renderPass;
    _geometryPipeline = Manager::create<Pipeline>(pipelineInfo);
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
    std::vector<component::EntityId> entities = component::Manager::getNoPrototypeView();

    _geometryPipeline->begin();
    {
        std::shared_ptr<OpenGLShaderGroup> shader = std::static_pointer_cast<OpenGLShaderGroup>(_geometryPipeline->getShaderGroup());

        mat4 m(1.0f);
        glDisable(GL_DEPTH_TEST);
        shader->setMat4("model", m);
        shader->setMat4("projection", m);
        shader->setMat4("view", m);
        shader->setInt("entityId", -1);
        Manager::getRendererAPI()->renderQuad3();
        glEnable(GL_DEPTH_TEST);
        glClear(GL_DEPTH_BUFFER_BIT); // XXX Not sure why but it only works in the browser if clear the depth buffer

        // Render entities
        shader->setMat4("projection", transpose(viewport->getCamera()->getProj()));
        shader->setMat4("view", transpose(viewport->getCamera()->getView()));

        for (auto entity : entities) {
            component::Mesh* mesh = component::Manager::getEntityComponent<component::Mesh>(entity);
            component::Transform* transform = component::Manager::getEntityComponent<component::Transform>(entity);

            if (mesh && transform) {
                mat4 model = transpose(transform->getWorldTransform(entity));
                shader->setMat4("model", model);

                // component::EntityId
                shader->setInt("entityId", entity);
                maxEid = std::max((int)maxEid, entity);

                // Draw mesh
                Manager::getRendererAPI()->renderMesh(mesh->sid);
            }
        }

        // Get pixel id
        eid = _geometryPipeline->getRenderPass()->getFramebuffer()->readPixel(0, pos.x, pos.y);
    }
    _geometryPipeline->end();

    return eid > maxEid ? -1 : eid;
}

} // namespace atta::graphics
