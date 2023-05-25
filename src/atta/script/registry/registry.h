//--------------------------------------------------
// Atta Script Module
// registry.h
// Date: 2023-05-25
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_SCRIPT_REGISTRY_REGISTRY_H
#define ATTA_SCRIPT_REGISTRY_REGISTRY_H

namespace atta::script {

class Registry {
  public:
    enum class Type { WORLD = 0, CONTROLLER };

    Registry(std::string typeidName, size_t typeidHash);

    std::string getName() const;
    std::string getTypeidName() const;
    size_t getTypeidHash() const;
    static const std::vector<Registry*>& getRegistries();

  protected:
    void addRegistry(Registry* registry);
    void removeRegistry(Registry* registry);

    std::string _name;       ///< Script class name
    std::string _typeidName; ///< typeid(T).name()
    size_t _typeidHash;      ///< typeid(T).hash_code()
    Type _type;              ///< Script type

  private:
    /**
     * @brief Script registries
     *
     * There will be one registry for each registered script
     */
    static std::vector<Registry*> _registries;
};

inline std::ostream& operator<<(std::ostream& os, const Registry& r) { return os << r.getName(); }

} // namespace atta::script

#endif // ATTA_SCRIPT_REGISTRY_REGISTRY_H
