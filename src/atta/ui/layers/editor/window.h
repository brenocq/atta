//--------------------------------------------------
// Atta UI Module
// window.h
// Date: 2021-09-28
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_UI_LAYERS_EDITOR_WINDOW_H
#define ATTA_UI_LAYERS_EDITOR_WINDOW_H

#include <atta/component/interface.h>
#include <atta/utils/stringId.h>
#include <imgui.h>

namespace atta::ui {

template <typename T>
class Window {
  public:
    static void render();
    static void setOpen(bool open) { getInstance()._open = open; }

    static T& getInstance() {
        static T instance;
        return instance;
    }

    static std::string getName() { return getInstance()._name; }
    static StringId getSID() { return getInstance()._sid; }
    static bool getOpen() { return getInstance()._open; }

  protected:
    void setName(std::string name) {
        _sid = StringId(name);
        _name = name;
        _open = false;
        _initialSize = vec2(0.0f);
    }

    virtual void renderImpl() = 0;

    StringId _sid;
    std::string _name;
    vec2 _initialSize;
    bool _open;
};

template <typename T>
void Window<T>::render() {
    T& window = getInstance();
    if (window._open) {
        ImGui::SetNextWindowSize(ImVec2(window._initialSize.x, window._initialSize.y), ImGuiCond_FirstUseEver);
        ImGui::Begin(window._name.c_str(), &(window._open));
        window.renderImpl();
        ImGui::End();
    }
}

} // namespace atta::ui
#endif // ATTA_UI_LAYERS_EDITOR_WINDOW_H
