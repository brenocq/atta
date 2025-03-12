//--------------------------------------------------
// Atta File Module
// interface.inl
// Date: 2025-03-10
// By Breno Cunha Queiroz
//--------------------------------------------------

namespace atta::file {

template <typename T>
void registerComponentIO(const SerializeFunc& serialize, const DeserializeFunc& deserialize) {
    registerComponentIO(cmp::getId<T>(), serialize, deserialize);
}

template <typename T>
void getComponentIO(std::optional<SerializeFunc>& serialize, std::optional<DeserializeFunc>& deserialize) {
    getComponentIO(cmp::getId<T>(), serialize, deserialize);
}

} // namespace atta::file
