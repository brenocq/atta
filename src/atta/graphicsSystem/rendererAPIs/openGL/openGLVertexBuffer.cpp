//--------------------------------------------------
// Atta Graphics System
// openGLVertexBuffer.cpp
// Date: 2021-09-10
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/graphicsSystem/rendererAPIs/openGL/openGLVertexBuffer.h>

namespace atta
{
	OpenGLVertexBuffer::OpenGLVertexBuffer(const VertexBuffer::CreateInfo& info, OpenGLId vao):
		VertexBuffer(info), _id(0)
	{
		// Create VBO buffer
		glCreateBuffers(1, &_id);
		glNamedBufferData(_id, _size, _data, convertUsage(_usage));

		// Bind VBO to VAO (offset 0, stride from layout)
		glVertexArrayVertexBuffer(vao, 0, _id, 0, _layout.getStride());

		uint32_t i = 0;
		for(const auto& element : _layout.getElements())
		{
			GLenum openGLType = convertBaseType(element.type);

			// Define attribute format
			if(openGLType == GL_INT)
			{
				glVertexArrayAttribIFormat(vao, 
						i, 
						element.getComponentCount(), 
						openGLType, 
						element.offset);
			}
			else
			{
				glVertexArrayAttribFormat(vao, 
						i, 
						element.getComponentCount(), 
						openGLType, 
						element.normalized ? GL_TRUE : GL_FALSE,
						element.offset);
			}

			// Define which VBO that will use this attribute (only using one VBO)
			glVertexArrayAttribBinding(vao, i, 0);
			// Enable the attribute
			glEnableVertexArrayAttrib(vao, i);
			i++;
		}
	}

	OpenGLVertexBuffer::~OpenGLVertexBuffer()
	{
		if(_id > 0)
		{
			glDeleteBuffers(1, &_id);
			_id = 0;
		}
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

	GLenum OpenGLVertexBuffer::convertBaseType(VertexBufferElement::Type type)
	{
		switch(type)
		{
			case VertexBufferElement::Type::BOOL: return GL_BOOL;
			case VertexBufferElement::Type::INT: 
			case VertexBufferElement::Type::UINT:
			case VertexBufferElement::Type::IVEC2: 
			case VertexBufferElement::Type::IVEC3: 
			case VertexBufferElement::Type::IVEC4: return GL_INT;
			case VertexBufferElement::Type::FLOAT:
			case VertexBufferElement::Type::VEC2: 
			case VertexBufferElement::Type::VEC3:
			case VertexBufferElement::Type::VEC4:
			case VertexBufferElement::Type::MAT3:
			case VertexBufferElement::Type::MAT4: return GL_FLOAT;
		}
		ASSERT(false, "Unknown vertex buffer element type");
	}
}
