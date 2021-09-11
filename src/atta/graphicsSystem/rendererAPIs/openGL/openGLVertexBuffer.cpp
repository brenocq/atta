//--------------------------------------------------
// Atta Graphics System
// openGLVertexBuffer.cpp
// Date: 2021-09-10
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/graphicsSystem/rendererAPIs/openGL/openGLVertexBuffer.h>

namespace atta
{
	OpenGLVertexBuffer::OpenGLVertexBuffer(const VertexBuffer::CreateInfo& info):
		VertexBuffer(info), _id(0)
	{
		glCreateBuffers(1, &_id);
		glNamedBufferData(_id, _size, _data.data(), convertUsage(_usage));
	}

	OpenGLVertexBuffer::~OpenGLVertexBuffer()
	{
		if(_id > 0)
		{
			glDeleteBuffers(1, &_id);
			_id = 0;
		}
	}

	void OpenGLVertexBuffer::setData(uint8_t* data, uint32_t size, uint32_t offset)
	{
		_size = size;
		_data = std::vector<uint8_t>(data, data+_size);
		glNamedBufferSubData(_id, offset, _size, _data.data());
	}

	void OpenGLVertexBuffer::bind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, _id);
	}

	GLenum OpenGLVertexBuffer::convertUsage(Usage usage)
	{
		switch(usage)
		{
			case Usage::STATIC: return GL_STATIC_DRAW;
			case Usage::DYNAMIC: return GL_DYNAMIC_DRAW;
		}
		ASSERT(false, "Unknown vertex buffer usage");
	}
}
