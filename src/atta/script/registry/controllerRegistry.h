//--------------------------------------------------
// Atta Script Module
// controllerRegistry.h
// Date: 2023-05-25
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_SCRIPT_REGISTRY_CONTROLLER_REGISTRY_H
#define ATTA_SCRIPT_REGISTRY_CONTROLLER_REGISTRY_H

#include <atta/component/interface.h>
#include <atta/script/registry/registry.h>

// clang-format off
#define ATTA_REGISTER_CONTROLLER(TYPE) inline ::atta::script::TypedControllerRegistry<TYPE> controllerRegistry##TYPE(#TYPE);
// clang-format on

namespace atta::script {

class ControllerRegistry : public Registry {
  public:
    static const std::vector<const ControllerRegistry*>& getRegistries();
    static const ControllerRegistry* getRegistry(StringId sid);

    virtual void run(cmp::Entity entity, float dt, uint32_t num = 1) const = 0;

  protected:
    ControllerRegistry(std::string name, std::string typeidName, size_t typeidHash);

    void addRegistry(const ControllerRegistry* registry);
    void removeRegistry(const ControllerRegistry* registry);

    /**
     * @brief Controller script registries
     *
     * We need to ensure that this vector is initialized in the first time addRegistry is called. Because the order of initialized of static objects
     * in the translation unit is not known, we need to have this vector as an static in a function.
     *
     * There will be one registry for each registered controller script
     *
     * @return Vector of controller script registries
     */
    static std::vector<const ControllerRegistry*>& getRegs();
};

template <typename T>
class TypedControllerRegistry : public ControllerRegistry {
  public:
    TypedControllerRegistry(std::string name);
    ~TypedControllerRegistry();

    void run(cmp::Entity entity, float dt, uint32_t num) const override;
};

} // namespace atta::script

#include <atta/script/registry/controllerRegistry.inl>

#endif // ATTA_SCRIPT_REGISTRY_CONTROLLER_REGISTRY_H
