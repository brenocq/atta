//--------------------------------------------------
// Atta Script Module
// controllerRegistry.h
// Date: 2023-05-25
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_SCRIPT_REGISTRY_CONTROLLER_REGISTRY_H
#define ATTA_SCRIPT_REGISTRY_CONTROLLER_REGISTRY_H

#include <atta/script/registry/registry.h>
#include <atta/component/interface.h>

// clang-format off
#define ATTA_REGISTER_CONTROLLER(TYPE) inline ::atta::script::TypedControllerRegistry<TYPE> controllerRegistry##TYPE {};
// clang-format on

namespace atta::script {

class ControllerRegistry : public Registry {
  public:
    static const std::vector<ControllerRegistry*>& getRegistries();

    // virtual void runController(cmp::Entity entity, float dt, uint32_t num = 1);

  protected:
    ControllerRegistry(std::string typeidName, size_t typeidHash);

    void addRegistry(ControllerRegistry* registry);
    void removeRegistry(ControllerRegistry* registry);

  private:
    /**
     * @brief Controller script registries
     *
     * There will be one registry for each registered controller script
     */
    static std::vector<ControllerRegistry*> _registries;
};

template <typename T>
class TypedControllerRegistry : public ControllerRegistry {
  public:
    TypedControllerRegistry();
    ~TypedControllerRegistry();
};

} // namespace atta::script

#include <atta/script/registry/controllerRegistry.inl>

#endif // ATTA_SCRIPT_REGISTRY_CONTROLLER_REGISTRY_H
