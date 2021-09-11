//--------------------------------------------------
// Atta Graphics System
// openGLPipeline.cpp
// Date: 2021-09-09
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/graphicsSystem/rendererAPIs/openGL/openGLPipeline.h>

namespace atta
{
	OpenGLPipeline::OpenGLPipeline(const Pipeline::CreateInfo& info):
		Pipeline(info), _vertexArrayId(0)
	{
		DASSERT(_renderPass, "Can not create pipeline without render pass");
		DASSERT(_shaderGroup, "Can not create pipeline without shader group");

		glGenVertexArrays(1, &_vertexArrayId);
		glBindVertexArray(_vertexArrayId);

		//uint32_t i = 0;
		//for(const auto& element : _layout.getElements())
		//{
		//	glEnableVertexAttribArray(i);
		//	GLenum openGLType = convertBaseType(element.type);

		//	if(openGLType == GL_INT)
		//	{
		//		glVertexAttribIPointer(i,
		//			element.getComponentCount(),
		//			openGLType,
		//			_layout.getStride(),
		//			(const void*)(intptr_t)element.offset);
		//	}
		//	else
		//	{
		//		glVertexAttribPointer(i,
		//			element.getComponentCount(),
		//			openGLType,
		//			element.normalized ? GL_TRUE : GL_FALSE,
		//			_layout.getStride(),
		//			(const void*)(intptr_t)element.offset);
		//	}
		//	i++;
		//}

		glBindVertexArray(0);
	}

	OpenGLPipeline::~OpenGLPipeline()
	{
		if(_vertexArrayId)
		{
			glDeleteVertexArrays(1, &_vertexArrayId);
			_vertexArrayId = 0;
		}
	}

	void OpenGLPipeline::bind()
	{
		glBindVertexArray(_vertexArrayId);
	}

	GLenum OpenGLPipeline::convertBaseType(VertexBufferElement::Type type)
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
