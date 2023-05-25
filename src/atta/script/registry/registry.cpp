//--------------------------------------------------
// Atta Script Module
// registry.cpp
// Date: 2023-05-25
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/script/registry/registry.h>

namespace atta::script {

Registry::Registry(std::string name, std::string typeidName, size_t typeidHash) : _name(name), _typeidName(typeidName), _typeidHash(typeidHash) {}

std::string Registry::getName() const { return _name; }

std::string Registry::getTypeidName() const { return _typeidName; }

size_t Registry::getTypeidHash() const { return _typeidHash; }

} // namespace atta::script
