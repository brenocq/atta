//--------------------------------------------------
// Atta Script Module
// worldRegistry.h
// Date: 2023-05-25
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_SCRIPT_REGISTRY_WORLD_REGISTRY_H
#define ATTA_SCRIPT_REGISTRY_WORLD_REGISTRY_H

#include <atta/script/registry/registry.h>

// clang-format off
#define ATTA_REGISTER_WORLD(TYPE) inline ::atta::script::TypedWorldRegistry<TYPE> worldRegistry##TYPE(#TYPE);
// clang-format on

namespace atta::script {

class WorldRegistry : public Registry {
  public:
    static const std::vector<WorldRegistry*>& getRegistries();

    static void onLoad();
    static void onUnload();
    static void onStart();
    static void onStop();
    static void onContinue();
    static void onPause();
    static void onUpdateBefore(float dt);
    static void onUpdateAfter(float dt);
    static void onUIRender();
    static void onAttaLoop();

  protected:
    WorldRegistry(std::string name, std::string typeidName, size_t typeidHash);

    void addRegistry(WorldRegistry* registry);
    void removeRegistry(WorldRegistry* registry);

    virtual void runOnLoad() = 0;
    virtual void runOnUnload() = 0;
    virtual void runOnStart() = 0;
    virtual void runOnStop() = 0;
    virtual void runOnContinue() = 0;
    virtual void runOnPause() = 0;
    virtual void runOnUpdateBefore(float dt) = 0;
    virtual void runOnUpdateAfter(float dt) = 0;
    virtual void runOnUIRender() = 0;
    virtual void runOnAttaLoop() = 0;


  private:
    /**
     * @brief World script registries
     *
     * We need to ensure that this vector is initialized in the first time addRegistry is called. Because the order of initialized of static objects
     * in the translation unit is not known, we need to have this vector as an static in a function.
     *
     * There will be one registry for each registered world script
     *
     * @return Vector of world script registries
     */
    static std::vector<WorldRegistry*>& getRegs();
};

template <typename T>
class TypedWorldRegistry : public WorldRegistry {
  public:
    TypedWorldRegistry(std::string name);
    ~TypedWorldRegistry();

    void runOnLoad() override;
    void runOnUnload() override;
    void runOnStart() override;
    void runOnStop() override;
    void runOnContinue() override;
    void runOnPause() override;
    void runOnUpdateBefore(float dt) override;
    void runOnUpdateAfter(float dt) override;
    void runOnUIRender() override;
    void runOnAttaLoop() override;

  private:
    T _worldScript;
};

} // namespace atta::script

#include <atta/script/registry/worldRegistry.inl>

#endif // ATTA_SCRIPT_REGISTRY_WORLD_REGISTRY_H