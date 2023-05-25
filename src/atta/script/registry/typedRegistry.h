//--------------------------------------------------
// Atta Script Module
// typedRegistry.h
// Date: 2023-05-25
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_SCRIPT_REGISTRY_TYPED_REGISTRY_H
#define ATTA_SCRIPT_REGISTRY_TYPED_REGISTRY_H

#include <atta/script/registry/registry.h>

namespace atta::script {

template <typename T>
class TypedRegistry : public Registry {
  public:
    TypedRegistry();
    ~TypedRegistry();
};

} // namespace atta::script

#include <atta/script/registry/typedRegistry.inl>

#endif // ATTA_SCRIPT_REGISTRY_TYPED_REGISTRY_H
