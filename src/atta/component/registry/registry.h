//--------------------------------------------------
// Atta Component Module
// registry.h
// Date: 2021-11-05
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_COMPONENT_REGISTRY_REGISTRY_H
#define ATTA_COMPONENT_REGISTRY_REGISTRY_H
#include <atta/component/base.h>
#include <atta/component/components/component.h>
#include <atta/utils/stringId.h>

namespace atta::component {
class Manager;
class Registry {
  public:
    using Type = StringId;
    Registry(unsigned sizeofT, std::string typeidName, size_t typeidHash)
        : _sizeof(sizeofT), _typeidName(typeidName), _typeidHash(typeidHash), _index(0), _poolCreated(false) {}

    static const std::vector<Registry*>& get();
    static const Registry* get(ComponentId cid);

    virtual void renderUI(Component* component) = 0;

    virtual void serialize(std::ostream& os, Component* component) = 0;
    virtual void deserialize(std::istream& is, Component* component) = 0;
    unsigned getSerializedSize(Component* component);

    virtual ComponentDescription& getDescription() = 0;
    virtual std::vector<uint8_t> getDefault() = 0;
    unsigned getSizeof() const;
    std::string getTypeidName() const;
    size_t getTypeidHash() const;
    ComponentId getId() const;
    unsigned getIndex() const;
    bool getPoolCreated() const;
    void setPoolCreated(bool poolCreated);

  protected:
    template <AttributeType attributeType>
    void renderUIAttribute(AttributeDescription aDesc, void* d, unsigned size, std::string imguiId) {}
    void renderUIAttribute(AttributeDescription aDesc, void* d, unsigned size, std::string imguiId);

    unsigned _sizeof;        ///< sizeof(T)
    std::string _typeidName; ///< typeid(T).name()
    size_t _typeidHash;      ///< typeid(T).hash_code()

    unsigned _index; ///< Each component receives an unique index
    bool _poolCreated;

    static std::vector<Registry*>& getRegistries();

  private:
    void setIndex(unsigned index) { _index = index; }
    friend Manager;
};

inline std::ostream& operator<<(std::ostream& os, Registry& c) { return os << c.getDescription().name; }

} // namespace atta::component

#include <atta/component/registry/registry.inl>

#endif // ATTA_COMPONENT_REGISTRY_REGISTRY_H
