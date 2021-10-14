//--------------------------------------------------
// Atta Graphics System
// window.h
// Date: 2021-08-16
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_GRAPHICS_SYSTEM_WINDOW_H
#define ATTA_GRAPHICS_SYSTEM_WINDOW_H
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <atta/core/stringId.h>
#include <atta/eventSystem/eventManager.h>

namespace atta
{
    class Window final
    {
    public:
        struct CreateInfo
        {
            std::string title = "Atta - by Brenocq";
            size_t width = 1600;
            size_t height = 900;
            //std::shared_ptr<EventManager> eventManager;
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
        ~Window();

        size_t getWidth() const { return _width; }
        size_t getHeight() const { return _height; }

        void update();
        void swapBuffers();

        GLFWwindow* getGLFWHandle() const { return _window; }

    private:

        GLFWwindow* _window;
        std::string _title;
        size_t _width, _height;
        static unsigned _glfwWindowCounter;
        //std::shared_ptr<EventManager> _eventManager;
    };
}

#endif// ATTA_GRAPHICS_SYSTEM_WINDOW_H
