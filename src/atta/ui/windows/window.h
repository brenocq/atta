//--------------------------------------------------
// Atta UI Module
// window.h
// Date: 2021-09-28
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_UI_WINDOWS_WINDOW_H
#define ATTA_UI_WINDOWS_WINDOW_H

#include <atta/component/interface.h>
#include <atta/ui/editor.h>
#include <atta/ui/manager.h>
#include <atta/utils/stringId.h>
#include <imgui.h>

namespace atta::ui {

template <typename T>
class Window {
  public:
    enum class DockPosition { LEFT, RIGHT, UP, DOWN, CENTER, FLOATING };

    static T& getInstance() {
        static T instance;
        return instance;
    }

    static void render();

    static void setOpen(bool open) { getInstance()._open = open; }
    static bool getOpen() { return getInstance()._open; }

    static std::string getName() { return getInstance()._name; }
    static StringId getSID() { return getInstance()._sid; }

    static ImGuiID getDockId(DockPosition position) {
        const Editor& editor = ui::Manager::getInstance().getEditor();
        switch (position) {
            case DockPosition::LEFT:
                return editor.getLeftDockId();
            case DockPosition::RIGHT:
                return editor.getRightDockId();
            case DockPosition::UP:
                return editor.getCenterDockId();
            case DockPosition::DOWN:
                return editor.getDownDockId();
            case DockPosition::CENTER:
                return editor.getCenterDockId();
            case DockPosition::FLOATING:
                LOG_WARN("ui::Window", "It is not possible to [w]getDockId[] from DockPosition::FLOATING");
                return 0;
        }
        return 0;
    }

  protected:
    void initialize(std::string name, DockPosition dockPosition = DockPosition::LEFT, vec2 size = vec2(0.0f), vec2 position = vec2(100.0f)) {
        _sid = StringId(name);
        _name = name;
        _open = false;
        _initialDockPosition = dockPosition;
        _initialSize = size;
        _initialPosition = position;
    }

    void setName(std::string name) { _name = name; }

    virtual void renderImpl() = 0;

    StringId _sid;
    std::string _name;
    bool _open;

    // Initial window state
    vec2 _initialSize;
    vec2 _initialPosition; // Only valid if _initialDockPosition is FLOATING
    DockPosition _initialDockPosition;
};

template <typename T>
void Window<T>::render() {
    T& window = getInstance();
    if (window._open) {
        // Setup size
        ImGui::SetNextWindowSize(ImVec2(window._initialSize.x, window._initialSize.y), ImGuiCond_FirstUseEver);

        // Setup dock/floating position
        if (window._initialDockPosition != DockPosition::FLOATING)
            ImGui::SetNextWindowDockID(getDockId(window._initialDockPosition), ImGuiCond_FirstUseEver);
        else
            ImGui::SetNextWindowPos(ImVec2(window._initialPosition.x, window._initialPosition.y), ImGuiCond_FirstUseEver);

        // Render window
        ImGui::Begin((window._name + "##Atta").c_str(), &(window._open));
        window.renderImpl();
        ImGui::End();
    }
}

} // namespace atta::ui
#endif // ATTA_UI_WINDOWS_WINDOW_H
