//--------------------------------------------------
// Atta Graphics System
// renderPass.h
// Date: 2021-09-09
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_GRAPHICS_SYSTEM_RENDER_PASS_H
#define ATTA_GRAPHICS_SYSTEM_RENDER_PASS_H
#include <atta/core/stringId.h>
#include <atta/graphicsSystem/framebuffer.h>

namespace atta
{
	class RenderPass
	{
	public:
		struct CreateInfo
		{
			std::shared_ptr<Framebuffer> framebuffer;
			StringId debugName = StringId("Unnamed Render Pass");
		};

		RenderPass(const CreateInfo& info);
		virtual ~RenderPass() = default;

		virtual void begin() = 0;
		virtual void end() = 0;

		std::shared_ptr<Framebuffer> getFramebuffer() const { return _framebuffer; }

	protected:
		std::shared_ptr<Framebuffer> _framebuffer;

		const StringId _debugName;
	};
}

#endif// ATTA_GRAPHICS_SYSTEM_RENDER_PASS_H
