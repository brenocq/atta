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

  protected:
    Registry(unsigned sizeofT, std::string typeidName, size_t typeidHash)
        : _sizeof(sizeofT), _typeidName(typeidName), _typeidHash(typeidHash), _id(0) {}

    template <AttributeType attributeType>
    void renderUIAttribute(AttributeDescription aDesc, void* d, unsigned size, std::string imguiId) {}
    void renderUIAttribute(AttributeDescription aDesc, void* d, unsigned size, std::string imguiId);

    unsigned _sizeof;        ///< sizeof(T)
    std::string _typeidName; ///< typeid(T).name()
    size_t _typeidHash;      ///< typeid(T).hash_code()

    /**
     * @brief Each component receives an unique id
     *
     * The id is the index in the vector of components
     */
    ComponentId _id;

    static std::vector<Registry*>& getRegistries();

  private:
    friend Manager;
};

inline std::ostream& operator<<(std::ostream& os, Registry& c) { return os << c.getDescription().name; }

} // namespace atta::component

#include <atta/component/registry/registry.inl>

#endif // ATTA_COMPONENT_REGISTRY_REGISTRY_H
