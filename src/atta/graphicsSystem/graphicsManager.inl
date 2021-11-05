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

    template<> std::shared_ptr<Image> GraphicsManager::createImpl<Image>(Image::CreateInfo info);
    template<> std::shared_ptr<Framebuffer> GraphicsManager::createImpl<Framebuffer>(Framebuffer::CreateInfo info);
    template<> std::shared_ptr<IndexBuffer> GraphicsManager::createImpl<IndexBuffer>(IndexBuffer::CreateInfo info);
    template<> std::shared_ptr<VertexBuffer> GraphicsManager::createImpl<VertexBuffer>(VertexBuffer::CreateInfo info);
    template<> std::shared_ptr<RenderPass> GraphicsManager::createImpl<RenderPass>(RenderPass::CreateInfo info);
    template<> std::shared_ptr<Shader> GraphicsManager::createImpl<Shader>(Shader::CreateInfo info);
    template<> std::shared_ptr<ShaderGroup> GraphicsManager::createImpl<ShaderGroup>(ShaderGroup::CreateInfo info);
    template<> std::shared_ptr<Pipeline> GraphicsManager::createImpl<Pipeline>(Pipeline::CreateInfo info);

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
        return nullptr;
    }
}
