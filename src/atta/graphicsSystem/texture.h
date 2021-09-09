//--------------------------------------------------
// Atta Graphics System
// image.h
// Date: 2021-09-09
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_GRAPHICS_SYSTEM_IMAGE_H
#define ATTA_GRAPHICS_SYSTEM_IMAGE_H
#include <atta/core/stringId.h>

namespace atta
{
	class Image
	{
	public:
		enum class Format
		{
			NONE,
			RGB,// 3 bytes
			RGBA,// 4 bytes
		};

		struct CreateInfo
		{
			bool backfaceCulling = true;
			bool wireframe = false;
			bool lineWidth = 1.0f;

			StringId sid = StringId("unnamedImage");
		};

		Image(const CreateInfo& info);
		virtual ~Image() = default;

	protected:
		uint32_t _width;
		uint32_t _height;

		const StringId _sid;
	};
}

#endif// ATTA_GRAPHICS_SYSTEM_IMAGE_H
