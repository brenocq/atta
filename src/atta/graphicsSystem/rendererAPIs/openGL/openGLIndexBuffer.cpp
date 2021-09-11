//--------------------------------------------------
// Atta Graphics System
// openGLIndexBuffer.cpp
// Date: 2021-09-10
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/graphicsSystem/rendererAPIs/openGL/openGLIndexBuffer.h>

namespace atta
{
	OpenGLIndexBuffer::OpenGLIndexBuffer(const IndexBuffer::CreateInfo& info, OpenGLId vao):
		IndexBuffer(info), _id(0)
	{
		glCreateBuffers(1, &_id);
		glNamedBufferData(_id, _size, _data, GL_STATIC_DRAW);

		// Bind EBO to VAO (offset 0, stride of one index)
		glVertexArrayVertexBuffer(vao, 1, _id, 0, sizeof(uint32_t));
	}

	OpenGLIndexBuffer::~OpenGLIndexBuffer()
	{
		if(_id > 0)
		{
			glDeleteBuffers(1, &_id);
			_id = 0;
		}
	}

	void OpenGLIndexBuffer::bind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _id);
	}
}
