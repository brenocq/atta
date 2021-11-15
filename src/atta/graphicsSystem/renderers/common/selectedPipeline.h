//--------------------------------------------------
// Atta Graphics System
// selectedPipeline.h
// Date: 2021-11-15
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_GRAPHICS_SYSTEM_RENDERERS_COMMON_SELECTED_PIPELINE_H
#define ATTA_GRAPHICS_SYSTEM_RENDERERS_COMMON_SELECTED_PIPELINE_H
#include <atta/graphicsSystem/pipeline.h>
#include <atta/graphicsSystem/cameras/camera.h>

namespace atta
{
    class SelectedPipeline final
    {
    public:
        SelectedPipeline(std::shared_ptr<RenderPass> renderPass, VertexBufferLayout layout);

        void render(std::shared_ptr<Camera> camera);

    private:
        std::shared_ptr<Pipeline> _pipeline;
    };
}

#endif// ATTA_GRAPHICS_SYSTEM_RENDERERS_COMMON_SELECTED_PIPELINE_H
