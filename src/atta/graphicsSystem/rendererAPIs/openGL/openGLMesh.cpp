//--------------------------------------------------
// Atta Graphics System
// openGLMesh.cpp
// Date: 2021-09-10
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/graphicsSystem/rendererAPIs/openGL/openGLMesh.h>
#include <atta/resourceSystem/resourceManager.h>
#include <atta/resourceSystem/resources/mesh.h>

namespace atta
{
	OpenGLMesh::OpenGLMesh(StringId sid):
		_sid(sid), _id(0)
	{
		Mesh* mesh = ResourceManager::get<Mesh>(sid.getString());

		glCreateVertexArrays(1, &_id); 
		glBindVertexArray(_id);
		LOG_DEBUG("OpenGLMesh", "Generated: $0", _id);

		VertexBuffer::CreateInfo vertexInfo {};
		vertexInfo.data = reinterpret_cast<const uint8_t*>(mesh->getVertices().data());
		vertexInfo.size = mesh->getVertices().size()*sizeof(mesh->getVertices()[0]);
		vertexInfo.layout = {
			{ "inPosition", VertexBufferElement::Type::VEC3 },
			{ "inNormal", VertexBufferElement::Type::VEC3 },
			{ "inTexCoord", VertexBufferElement::Type::VEC2 }
		};
		_vertexBuffer = std::make_shared<OpenGLVertexBuffer>(vertexInfo, _id);

		IndexBuffer::CreateInfo indexInfo {};
		indexInfo.data = reinterpret_cast<const uint8_t*>(mesh->getIndices().data());
		indexInfo.size = mesh->getIndices().size()*sizeof(mesh->getIndices()[0]);
		_indexBuffer = std::make_shared<OpenGLIndexBuffer>(indexInfo, _id);

		glBindVertexArray(0);

		LOG_DEBUG("OpenGLMesh", "OpenGl mesh created -> vertexSize = $0 | indexSize = $1", 
				sizeof(mesh->getVertices()[0]), sizeof(mesh->getIndices()[0]));
	}

	OpenGLMesh::~OpenGLMesh()
	{
		if(_id > 0)
			glDeleteVertexArrays(1, &_id);
	}

	void OpenGLMesh::bind()
	{
		LOG_DEBUG("OpenGLMesh", "Bind VAO $0", _id);
		glBindVertexArray(_id);
		LOG_DEBUG("OpenGLMesh", "Ok");
	}

	void OpenGLMesh::draw()
	{
		LOG_DEBUG("OpenGLMesh", "Draw");
		glDrawElements(GL_TRIANGLES, _indexBuffer->getCount(), GL_UNSIGNED_INT, 0);
		LOG_DEBUG("OpenGLMesh", "Draw ok");
	}
}
