//--------------------------------------------------
// Atta Graphics System
// viewport.cpp
// Date: 2021-09-14
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/graphicsSystem/viewport.h>

namespace atta
{
    Viewport::Viewport(CreateInfo info):
        _sid(info.sid), _renderer(info.renderer), _camera(info.camera)
    {
        ASSERT(_renderer != nullptr, "Can not create viewport ([w]$0[]) without renderer", _sid);
        ASSERT(_camera != nullptr, "Can not create viewport ([w]$0[]) without camera", _sid);
    }

    Viewport::~Viewport()
    {

    }

    void Viewport::render()
    {
        if(_newRenderer)
        {
            _renderer.reset();
            _renderer = _newRenderer;
            _newRenderer.reset();
        }
        _renderer->render(_camera);
    }

    void Viewport::resize(uint32_t width, uint32_t height)
    {
        _camera->setRatio(width/(float)height);
        _renderer->resize(width, height);
    }
}
