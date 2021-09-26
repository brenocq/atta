//--------------------------------------------------
// Atta Graphics System
// openGLFramebuffer.h
// Date: 2021-09-09
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_GRAPHICS_SYSTEM_RENDERER_APIS_OPENGL_OPENGL_FRAMEBUFFER_H
#define ATTA_GRAPHICS_SYSTEM_RENDERER_APIS_OPENGL_OPENGL_FRAMEBUFFER_H
#include <atta/graphicsSystem/framebuffer.h>
#include <atta/graphicsSystem/rendererAPIs/openGL/base.h>

namespace atta
{
	class OpenGLFramebuffer final : public Framebuffer
	{
	public:
		OpenGLFramebuffer(const Framebuffer::CreateInfo& info);
		~OpenGLFramebuffer();

		void bind(bool clear = true) override;
		void unbind() override;

		void resize(uint32_t width, uint32_t height, bool forceRecreate = false) override;

		static GLenum convertDepthAttachmentType(Image::Format format);

		//const std::vector<std::shared_ptr<Image>>& getColorAttachments() const { return _colorAttachments; };

	private:
		std::shared_ptr<Image> createColorAttachment(Image::Format format, int index);
		std::shared_ptr<Image> createDepthAttachment(Image::Format format);

		OpenGLId _id;

		// Attachments
		//std::vector<std::shared_ptr<Image>> _colorAttachments;
		//std::shared_ptr<Image> _depthAttachment;
		int _depthAttachmentIndex;

		// Attachment formats
		//std::vector<Image::Format> _colorAttachmentFormats;
		//Image::Format _depthAttachmentFormat;
	};
}

#endif// ATTA_GRAPHICS_SYSTEM_RENDERER_APIS_OPENGL_OPENGL_FRAMEBUFFER_H
