//--------------------------------------------------
// Atta Graphics System
// graphicsManager.inl
// Date: 2021-09-10
// By Breno Cunha Queiroz
//--------------------------------------------------

namespace atta
{
	template <typename T, typename... Args>
	std::shared_ptr<T> GraphicsManager::createImpl(Args... args)
	{
		ASSERT(false, "Trying to create invalid $0, which does not have an API specific implementation. Invalid class or invalid constructor parameters", typeid(T).name());
	}

	template <typename T, typename TOpenGL, typename TVulkan, typename... Args>
	std::shared_ptr<T> GraphicsManager::createSpecific(Args... args)
	{
		// If implementation exists, compile to cast, if didn't, compile to error message
		switch(_rendererAPI->getType())
		{
			case RendererAPI::OPENGL:
			{
				if constexpr(!std::is_same_v<T, TOpenGL>)
					return std::static_pointer_cast<T>(std::make_shared<TOpenGL>(args...));
				else
					ASSERT(false, "Trying to create $0, which does not have OpenGL implementation", typeid(T).name());
			}
			case RendererAPI::VULKAN:
			{
				if constexpr(!std::is_same_v<T, TVulkan>)
					return std::static_pointer_cast<T>(std::make_shared<TVulkan>(args...));
				else
					ASSERT(false, "Trying to create $0, which does not have Vulkan implementation", typeid(T).name());
			}
		}
	}
}
