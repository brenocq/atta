//--------------------------------------------------
// Atta Graphics System
// selectedPipeline.cpp
// Date: 2021-11-15
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/graphicsSystem/renderers/common/selectedPipeline.h>
#include <atta/graphicsSystem/graphicsManager.h>
#include <atta/graphicsSystem/framebuffer.h>
#include <atta/graphicsSystem/renderPass.h>

#include <atta/graphicsSystem/rendererAPIs/openGL/openGLShaderGroup.h>

#include <atta/componentSystem/componentManager.h>
#include <atta/componentSystem/components/meshComponent.h>
#include <atta/componentSystem/components/transformComponent.h>

namespace atta
{
    SelectedPipeline::SelectedPipeline(std::shared_ptr<RenderPass> renderPass, VertexBufferLayout layout)
    {
        ShaderGroup::CreateInfo shaderGroupInfo {};
        shaderGroupInfo.shaderPaths = {"shaders/common/selected.vert", "shaders/common/selected.frag"};
        shaderGroupInfo.debugName = StringId("SelectedPipeline Shader Group");
        std::shared_ptr<ShaderGroup> shaderGroup = GraphicsManager::create<ShaderGroup>(shaderGroupInfo);

        Pipeline::CreateInfo pipelineInfo {};
        pipelineInfo.shaderGroup = shaderGroup;
        pipelineInfo.layout = layout;
        pipelineInfo.renderPass = renderPass;
        _pipeline = GraphicsManager::create<Pipeline>(pipelineInfo);
    }

    void SelectedPipeline::render(std::shared_ptr<Camera> camera)
    {
        _pipeline->begin(false);
        {
            glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            glEnable(GL_BLEND);
            glClear(GL_STENCIL_BUFFER_BIT);

            std::shared_ptr<OpenGLShaderGroup> shader = std::static_pointer_cast<OpenGLShaderGroup>(_pipeline->getShaderGroup());
            shader->setMat4("projection", transpose(camera->getProj()));
            shader->setMat4("view", transpose(camera->getView()));

            EntityId entity = ComponentManager::getSelectedEntity();
            if(entity != -1)
            {
                MeshComponent* mesh = ComponentManager::getEntityComponent<MeshComponent>(entity);
                TransformComponent* transform = ComponentManager::getEntityComponent<TransformComponent>(entity);

                if(mesh && transform)
                {
                    // Draw mesh normal size
                    mat4 model = transform->getWorldTransform(entity); 
                    vec3 pos, scale;
                    quat ori;
                    model.getPosOriScale(pos, ori, scale);
                    model.transpose();

                    shader->setMat4("model", model);
                    shader->setVec4("color", vec4(0, 0, 0, 0));

                    glDisable(GL_DEPTH_TEST);
                    glStencilFunc(GL_ALWAYS, 1, 0xFF); 
                    glStencilMask(0xFF);// Update stencil
                    GraphicsManager::getRendererAPI()->renderMesh(mesh->sid);

                    // Draw scaled mesh
                    float distance = (camera->getPosition()-pos).length();
                    model.setPosOriScale(pos, ori, scale+distance/100.0f);
                    model.transpose();
                    shader->setMat4("model", model);
                    shader->setVec4("color", vec4(0.22f, 0.63f, 0.27f, 1.0f));

                    glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
                    glStencilMask(0x00);// Do not update stencil
                    GraphicsManager::getRendererAPI()->renderMesh(mesh->sid);

                    glEnable(GL_DEPTH_TEST);
                    glStencilMask(0xFF);
                    glStencilFunc(GL_ALWAYS, 1, 0xFF);
                }
            }
        }
        _pipeline->end();
    }
}
