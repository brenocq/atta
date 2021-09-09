//--------------------------------------------------
// Atta Graphics System
// framebuffer.h
// Date: 2021-09-09
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_GRAPHICS_SYSTEM_FRAMEBUFFER_H
#define ATTA_GRAPHICS_SYSTEM_FRAMEBUFFER_H
#include <atta/core/stringId.h>
#include <atta/core/math/vector.h>
#include <atta/graphicsSystem/image.h>

namespace atta
{
	class Framebuffer
	{
	public:
		struct Attachment
		{
			Image::Format format = Image::Format::RGBA;
		};

		struct CreateInfo
		{
			uint32_t width;
			uint32_t height;
			vec4 clearColor = { 0.0f, 0.0f, 0.0f, 0.0f };
			bool clearOnLoad = true;

			std::vector<Attachment> attachments = {};
			uint32_t samples = 1;
			std::map<uint32_t, std::shared_ptr<Image>> images;// Each image is mapped to one id


			StringId debugName = StringId("Unnamed Framebuffer");
		};

		Framebuffer(const CreateInfo& info);
		virtual ~Framebuffer() = default;

		virtual void bind() = 0;
		virtual void unbind() = 0;

		virtual void resize(uint32_t width, uint32_t height, bool forceRecreate = false) = 0;
		std::shared_ptr<Image> getImage(uint32_t attachment = 0);

	protected:
		uint32_t _width;
		uint32_t _height;
		vec4 _clearColor;
		bool _clearOnLoad;

		std::vector<Attachment> _attachments;
		uint32_t _samples;
		std::map<uint32_t, std::shared_ptr<Image>> _images;

		const StringId _debugName;
	};
}

#endif// ATTA_GRAPHICS_SYSTEM_FRAMEBUFFER_H
