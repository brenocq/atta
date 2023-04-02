//--------------------------------------------------
// Atta Graphics Module
// selectedPipeline.cpp
// Date: 2021-11-15
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/graphics/interface.h>
#include <atta/graphics/renderers/common/selectedPipeline.h>
#include <atta/graphics/apis/openGL/openGL.h>
#include <atta/component/components/mesh.h>
#include <atta/component/components/transform.h>
#include <atta/component/interface.h>

namespace atta::graphics {

SelectedPipeline::SelectedPipeline(std::shared_ptr<RenderPass> renderPass, VertexBufferLayout layout) {
    ShaderGroup::CreateInfo shaderGroupInfo{};
    shaderGroupInfo.shaderPaths = {"shaders/common/selected.vert", "shaders/common/selected.frag"};
    shaderGroupInfo.debugName = StringId("SelectedPipeline Shader Group");
    std::shared_ptr<ShaderGroup> shaderGroup = graphics::create<ShaderGroup>(shaderGroupInfo);

    Pipeline::CreateInfo pipelineInfo{};
    pipelineInfo.shaderGroup = shaderGroup;
    pipelineInfo.layout = layout;
    pipelineInfo.renderPass = renderPass;
    _pipeline = graphics::create<Pipeline>(pipelineInfo);
}

void SelectedPipeline::render(std::shared_ptr<Camera> camera) {
    _pipeline->begin(false);
    {
        glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_BLEND);
        glClear(GL_STENCIL_BUFFER_BIT);

        std::shared_ptr<ShaderGroup> shader = _pipeline->getShaderGroup();
        shader->setMat4("projection", transpose(camera->getProj()));
        shader->setMat4("view", transpose(camera->getView()));

        component::EntityId entity = component::getSelectedEntity();
        if (entity != -1) {
            component::Mesh* mesh = component::getComponent<component::Mesh>(entity);
            component::Transform* transform = component::getComponent<component::Transform>(entity);

            if (mesh && transform) {
                // Draw mesh normal size
                mat4 model = transform->getWorldTransformMatrix(entity);
                vec3 pos, scale;
                quat ori;
                model.getPosOriScale(pos, ori, scale);
                model.transpose();

                shader->setMat4("model", model);
                shader->setVec4("color", vec4(0, 0, 0, 0));

                glDisable(GL_DEPTH_TEST);
                glStencilFunc(GL_ALWAYS, 1, 0xFF);
                glStencilMask(0xFF); // Update stencil
                graphics::getGraphicsAPI()->renderMesh(mesh->sid);

                // Draw scaled mesh
                float distance = (camera->getPosition() - pos).length();
                model.setPosOriScale(pos, ori, scale + distance / 100.0f);
                model.transpose();
                shader->setMat4("model", model);
                shader->setVec4("color", vec4(0.22f, 0.63f, 0.27f, 1.0f));

                glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
                glStencilMask(0x00); // Do not update stencil
                graphics::getGraphicsAPI()->renderMesh(mesh->sid);

                glEnable(GL_DEPTH_TEST);
                glStencilMask(0xFF);
                glStencilFunc(GL_ALWAYS, 1, 0xFF);
            }
        }
    }
    _pipeline->end();
}

} // namespace atta::graphics
