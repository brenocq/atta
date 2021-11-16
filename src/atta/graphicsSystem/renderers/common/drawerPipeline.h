//--------------------------------------------------
// Atta Graphics System
// drawerPipeline.h
// Date: 2021-11-16
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_GRAPHICS_SYSTEM_RENDERERS_COMMON_DRAWER_PIPELINE_H
#define ATTA_GRAPHICS_SYSTEM_RENDERERS_COMMON_DRAWER_PIPELINE_H
#include <atta/graphicsSystem/pipeline.h>
#include <atta/graphicsSystem/cameras/camera.h>

namespace atta
{
    class DrawerPipeline final
    {
    public:
        DrawerPipeline(std::shared_ptr<RenderPass> renderPass);

        void render(std::shared_ptr<Camera> camera);

    private:
        std::shared_ptr<Pipeline> _linePipeline;
        std::shared_ptr<Pipeline> _pointPipeline;
    };
}

#endif// ATTA_GRAPHICS_SYSTEM_RENDERERS_COMMON_DRAWER_PIPELINE_H
