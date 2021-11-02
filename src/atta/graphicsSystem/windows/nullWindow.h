//--------------------------------------------------
// Atta Graphics System
// nullWindow.h
// Date: 2021-10-31
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_GRAPHICS_SYSTEM_WINDOWS_NULL_WINDOW_H
#define ATTA_GRAPHICS_SYSTEM_WINDOWS_NULL_WINDOW_H
#include <atta/graphicsSystem/windows/window.h>

namespace atta
{
    class NullWindow final : public Window
    {
    public:
        NullWindow(const CreateInfo& info);
        virtual ~NullWindow() = default;

        void update() override {}
        void swapBuffers() override {}
    };
}

#endif// ATTA_GRAPHICS_SYSTEM_WINDOWS_NULL_WINDOW_H
