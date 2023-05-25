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
    std::string getName() const;
    std::string getTypeidName() const;
    size_t getTypeidHash() const;

  protected:
    Registry(std::string name, std::string typeidName, size_t typeidHash);

    std::string _name;       ///< Script class name
    std::string _typeidName; ///< typeid(T).name()
    size_t _typeidHash;      ///< typeid(T).hash_code()
};

inline std::ostream& operator<<(std::ostream& os, const Registry& r) { return os << r.getName(); }

} // namespace atta::script

#endif // ATTA_SCRIPT_REGISTRY_REGISTRY_H
