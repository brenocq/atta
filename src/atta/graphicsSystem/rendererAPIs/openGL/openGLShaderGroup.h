//--------------------------------------------------
// Atta Graphics System
// openGLShaderGroup.h
// Date: 2021-09-09
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_GRAPHICS_SYSTEM_RENDERER_APIS_OPENGL_OPENGL_SHADER_GROUP_H
#define ATTA_GRAPHICS_SYSTEM_RENDERER_APIS_OPENGL_OPENGL_SHADER_GROUP_H
#include <atta/graphicsSystem/shaderGroup.h>
#include <atta/graphicsSystem/rendererAPIs/openGL/base.h>

namespace atta
{
	class OpenGLShaderGroup final : public ShaderGroup
	{
	public:
		OpenGLShaderGroup(const ShaderGroup::CreateInfo& info);
		~OpenGLShaderGroup();

		void recompile() override;

		void bind();

		OpenGLId getId() const { return _id; }
	
	private:
		OpenGLId _id;
	};
}

#endif// ATTA_GRAPHICS_SYSTEM_RENDERER_APIS_OPENGL_OPENGL_SHADER_GROUP_H
