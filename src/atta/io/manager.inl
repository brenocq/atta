//--------------------------------------------------
// Atta IO Module
// manager.inl
// Date: 2021-09-26
// By Breno Cunha Queiroz
//--------------------------------------------------

namespace atta::io {

template <typename T, typename... Args>
std::shared_ptr<T> Manager::createImpl(Args... args) {
    ASSERT(false,
           "Trying to create invalid $0, which does not have an OS specific implementation. Invalid class or invalid constructor parameters. Is it "
           "an IO device?",
           typeid(T).name());
}

template <>
std::shared_ptr<io::Camera> Manager::createImpl<io::Camera>(io::Camera::CreateInfo info);
template <>
std::shared_ptr<io::Serial> Manager::createImpl<io::Serial>(io::Serial::CreateInfo info);
template <>
std::shared_ptr<io::Bluetooth> Manager::createImpl<io::Bluetooth>(io::Bluetooth::CreateInfo info);

} // namespace atta::io
