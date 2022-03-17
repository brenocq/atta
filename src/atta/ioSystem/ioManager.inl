//--------------------------------------------------
// Atta IO System
// ioManager.inl
// Date: 2021-09-26
// By Breno Cunha Queiroz
//--------------------------------------------------

namespace atta::io
{
    template <typename T, typename... Args>
    std::shared_ptr<T> IOManager::createImpl(Args... args)
    {
        ASSERT(false, "Trying to create invalid $0, which does not have an OS specific implementation. Invalid class or invalid constructor parameters. Is it an IO device?", typeid(T).name());
    }

    template <> std::shared_ptr<io::Camera> IOManager::createImpl<io::Camera>(io::Camera::CreateInfo info);
    template <> std::shared_ptr<io::Serial> IOManager::createImpl<io::Serial>(io::Serial::CreateInfo info);
    template <> std::shared_ptr<io::Bluetooth> IOManager::createImpl<io::Bluetooth>(io::Bluetooth::CreateInfo info);
}
