//--------------------------------------------------
// Atta Graphics System
// indexBuffer.h
// Date: 2021-09-10
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_GRAPHICS_SYSTEM_INDEX_BUFFER_H
#define ATTA_GRAPHICS_SYSTEM_INDEX_BUFFER_H

namespace atta
{
	class IndexBuffer
	{
	public:
		struct CreateInfo
		{
			const uint8_t* data = nullptr;
			uint32_t size = 0;
			uint32_t offset = 0;
		};

		IndexBuffer(const CreateInfo& info);
		virtual ~IndexBuffer() = default;

		virtual void bind() const = 0;

		uint32_t getSize() const { return _size; };
		uint32_t getCount() const { return _size/sizeof(uint32_t); };

	protected:
		uint32_t _offset;
		uint32_t _size;
		const uint8_t* _data;
	};
}

#endif// ATTA_GRAPHICS_SYSTEM_INDEX_BUFFER_H
