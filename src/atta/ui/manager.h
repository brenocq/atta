//--------------------------------------------------
// Atta UI Module
// manager.h
// Date: 2021-09-28
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_UI_MANAGER_H
#define ATTA_UI_MANAGER_H

namespace atta::ui {

class Manager final {
  public:
    static Manager& getInstance();
    static void startUp();
    static void shutDown();

  private:
    void startUpImpl();
    void shutDownImpl();
};

} // namespace atta::ui

#endif // ATTA_UI_MANAGER_H
