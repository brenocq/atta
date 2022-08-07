//--------------------------------------------------
// Atta Component Module
// componentRegistry.h
// Date: 2021-11-05
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_COMPONENT_COMPONENT_REGISTRY_H
#define ATTA_COMPONENT_COMPONENT_REGISTRY_H
#include <atta/component/base.h>
#include <atta/component/components/component.h>
#include <atta/utils/stringId.h>

namespace atta::component {
class Manager;
class ComponentRegistry {
  public:
    using Type = StringId;
    ComponentRegistry(unsigned sizeofT, std::string typeidName, size_t typeidHash)
        : _sizeof(sizeofT), _typeidName(typeidName), _typeidHash(typeidHash), _index(0), _poolCreated(false) {}

    virtual void renderUI(Component* component) = 0;

    virtual void serialize(std::ostream& os, Component* component) = 0;
    virtual void deserialize(std::istream& is, Component* component) = 0;
    unsigned getSerializedSize(Component* component);

    virtual ComponentDescription& getDescription() = 0;
    virtual std::vector<uint8_t> getDefault() = 0;
    unsigned getSizeof() const { return _sizeof; }
    std::string getTypeidName() const { return _typeidName; }
    size_t getTypeidHash() const { return _typeidHash; }
    ComponentId getId() const { return COMPONENT_POOL_SSID_BY_NAME(_typeidName); }
    unsigned getIndex() const { return _index; }
    bool getPoolCreated() const { return _poolCreated; }
    void setPoolCreated(bool poolCreated) { _poolCreated = poolCreated; }

  protected:
    void registerToManager();

    template <AttributeType attributeType>
    void renderUIAttribute(AttributeDescription aDesc, void* d, unsigned size, std::string imguiId) {}
    void renderUIAttribute(AttributeDescription aDesc, void* d, unsigned size, std::string imguiId);

    unsigned _sizeof;        // sizeof(T)
    std::string _typeidName; // typeid(T).name()
    size_t _typeidHash;      // typeid(T).hash_code()

    // Component index starting from 0
    // This index is useful to access the entity component without iterating over the entity block
    unsigned _index;
    bool _poolCreated;

  private:
    void setIndex(unsigned index) { _index = index; }
    friend Manager;
};

//---------- Attribute helpers ----------//
inline std::ostream& operator<<(std::ostream& os, ComponentRegistry& c) { return os << c.getDescription().name; }
} // namespace atta::component

#include <atta/component/componentRegistry.inl>
#endif // ATTA_COMPONENT_COMPONENT_REGISTRY_H
