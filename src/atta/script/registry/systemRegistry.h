//--------------------------------------------------
// Atta Script Module
// systemRegistry.h
// Date: 2023-05-25
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_SCRIPT_REGISTRY_SYSTEM_REGISTRY_H
#define ATTA_SCRIPT_REGISTRY_SYSTEM_REGISTRY_H

#include <atta/script/registry/registry.h>

// clang-format off
#define ATTA_REGISTER_SYSTEM(TYPE) inline ::atta::script::TypedSystemRegistry<TYPE> systemRegistry##TYPE(#TYPE);
// clang-format on

namespace atta::script {

class SystemRegistry : public Registry {
  public:
    static const std::vector<SystemRegistry*>& getRegistries();

    static void onLoad();
    static void onUnload();
    static void onUIRender();
    static void onAttaLoop();

  protected:
    SystemRegistry(std::string name, std::string typeidName, size_t typeidHash);

    void addRegistry(SystemRegistry* registry);
    void removeRegistry(SystemRegistry* registry);

    virtual void runOnLoad() = 0;
    virtual void runOnUnload() = 0;
    virtual void runOnUIRender() = 0;
    virtual void runOnAttaLoop() = 0;

  private:
    /**
     * @brief System script registries
     *
     * We need to ensure that this vector is initialized in the first time addRegistry is called. Because the order of initialized of static objects
     * in the translation unit is not known, we need to have this vector as an static in a function.
     *
     * There will be one registry for each registered world script
     *
     * @return Vector of world script registries
     */
    static std::vector<SystemRegistry*>& getRegs();
};

template <typename T>
class TypedSystemRegistry : public SystemRegistry {
  public:
    TypedSystemRegistry(std::string name);
    ~TypedSystemRegistry();

    void runOnLoad() override;
    void runOnUnload() override;
    void runOnUIRender() override;
    void runOnAttaLoop() override;

  private:
    T _systemScript;
};

} // namespace atta::script

#include <atta/script/registry/systemRegistry.inl>

#endif // ATTA_SCRIPT_REGISTRY_SYSTEM_REGISTRY_H
