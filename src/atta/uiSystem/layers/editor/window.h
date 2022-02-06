//--------------------------------------------------
// Atta UI System
// window.h
// Date: 2021-09-28
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_UI_SYSTEM_LAYERS_EDITOR_WINDOW_H
#define ATTA_UI_SYSTEM_LAYERS_EDITOR_WINDOW_H
#include <atta/componentSystem/componentManager.h>
#include <atta/componentSystem/entity.h>
#include <atta/core/stringId.h>
#include <imgui.h>

namespace atta::ui
{
template<typename T>
    class Window
    {
    public:
        static void render();
        static void setOpen(bool open) { getInstance()._open = open; }

        static T& getInstance()
        {
            static T instance; 
            return instance; 
        }

        static std::string getName() { return getInstance()._name; }
        static StringId getSID() { return getInstance()._sid; }
        static bool getOpen() { return getInstance()._open; }

    protected:
        void setName(std::string name) 
        {
            _sid = StringId(name);
            _name = name;
            _open = false;
        }

        virtual void renderImpl() = 0;

        StringId _sid;
        std::string _name;
        bool _open;

    };

template <typename T>
    void Window<T>::render()
    {
        T& window = getInstance();
        if(window._open)
        {
            ImGui::Begin(window._name.c_str(), &(window._open));
            {
                window.renderImpl();
            }
            ImGui::End();
        }
    }
}
#endif// ATTA_UI_SYSTEM_LAYERS_EDITOR_WINDOW_H
