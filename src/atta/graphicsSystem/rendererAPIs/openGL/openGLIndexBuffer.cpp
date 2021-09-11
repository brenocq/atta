//--------------------------------------------------
// Atta Graphics System
// openGLIndexBuffer.cpp
// Date: 2021-09-10
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/graphicsSystem/rendererAPIs/openGL/openGLIndexBuffer.h>

namespace atta
{
	OpenGLIndexBuffer::OpenGLIndexBuffer(const IndexBuffer::CreateInfo& info):
		IndexBuffer(info), _id(0)
	{
		glCreateBuffers(1, &_id);
		glNamedBufferData(_id, _size, _data.data(), GL_STATIC_DRAW);
	}

	OpenGLIndexBuffer::~OpenGLIndexBuffer()
	{
		if(_id > 0)
		{
			glDeleteBuffers(1, &_id);
			_id = 0;
		}
	}

	void OpenGLIndexBuffer::setData(uint8_t* data, uint32_t size, uint32_t offset)
	{
		_size = size;
		_data = std::vector<uint8_t>(data, data+_size);
		glNamedBufferSubData(_id, offset, _size, _data.data());
	}

	void OpenGLIndexBuffer::bind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _id);
	}
}
