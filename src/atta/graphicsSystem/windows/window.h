//--------------------------------------------------
// Atta Graphics System
// window.h
// Date: 2021-10-31
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_GRAPHICS_SYSTEM_WINDOWS_WINDOW_H
#define ATTA_GRAPHICS_SYSTEM_WINDOWS_WINDOW_H
#include <atta/core/stringId.h>
#include <atta/eventSystem/eventManager.h>

namespace atta
{
    class Window
    {
    public:
        struct CreateInfo
        {
            std::string title = "Atta - by Brenocq";
            size_t width = 1600;
            size_t height = 900;
        };

        enum class Event : atta::Event::Type
        {
            MOUSE_MOVE = SID("Window_MouseMove"),
            MOUSE_BUTTON = SID("Window_MouseButton"),
            MOUSE_SCROLL = SID("Window_MouseScroll"),
            KEYBOARD_BUTTON = SID("Window_KeyboardButton"),
            RESIZE = SID("Window_Resize"),
            FOCUS = SID("Window_Focus"),
            MOVE = SID("Window_Move"),
            CLOSE = SID("Window_Close")
        };

        Window(const CreateInfo& info);
        virtual ~Window() = default;

        size_t getWidth() const { return _width; }
        size_t getHeight() const { return _height; }

        virtual void update() = 0;
        virtual void swapBuffers() = 0;

    protected:
        std::string _title;
        size_t _width, _height;
    };
}

#endif// ATTA_GRAPHICS_SYSTEM_WINDOWS_WINDOW_H
