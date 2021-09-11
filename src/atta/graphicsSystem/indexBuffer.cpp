//--------------------------------------------------
// Atta Graphics System
// indexBuffer.cpp
// Date: 2021-09-10
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/graphicsSystem/indexBuffer.h>

namespace atta
{
	IndexBuffer::IndexBuffer(const CreateInfo& info):
		_size(info.size), _offset(info.offset)
	{
		_data = std::vector<uint8_t>(info.data, info.data+_size);
	}
}
