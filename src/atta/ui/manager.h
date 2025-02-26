//--------------------------------------------------
// Atta UI Module
// manager.h
// Date: 2021-09-28
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_UI_MANAGER_H
#define ATTA_UI_MANAGER_H

#include <atta/graphics/interface.h>
#include <atta/ui/editor.h>

namespace atta::ui {

class Manager final {
  public:
    static Manager& getInstance();

    friend void startUp();
    friend void shutDown();

    //----- Custom component rendering -----//
    using ComponentUIFunc = std::function<void(cmp::Component*)>;
    friend void registerComponentUI(cmp::ComponentId cid, ComponentUIFunc renderFunc);
    friend std::optional<ComponentUIFunc> getComponentUI(cmp::ComponentId cid);

    friend const std::vector<std::shared_ptr<ui::Viewport>>& getViewports();
    friend void openViewportModal(StringId sid);
    friend void addViewport(std::shared_ptr<ui::Viewport> viewport);
    friend bool getViewportRendering();
    friend void setViewportRendering(bool viewportRendering);
    friend unsigned getViewportDockId();

  private:
    void startUpImpl();
    void shutDownImpl();

    void registerComponentUIImpl(cmp::ComponentId cid, ComponentUIFunc renderFunc);
    std::optional<ComponentUIFunc> getComponentUIImpl(cmp::ComponentId cid);

    const std::vector<std::shared_ptr<ui::Viewport>>& getViewportsImpl() const;
    void openViewportModalImpl(StringId sid);
    void addViewportImpl(std::shared_ptr<ui::Viewport> viewport);
    bool getViewportRenderingImpl() const;
    void setViewportRenderingImpl(bool viewportRendering);
    unsigned getViewportDockIdImpl();

    void setTheme();
    void initOpenGL();
    void initVulkan();

    void render();
    Editor _editor;

    std::map<cmp::ComponentId, ComponentUIFunc> _componentRenderFuncs;
};

} // namespace atta::ui

#endif // ATTA_UI_MANAGER_H
