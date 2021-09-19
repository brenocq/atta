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

		void setBool(const char* name, bool b) override;
		void setInt(const char* name, int i) override;
		void setFloat(const char* name, float f) override;
		void setVec2(const char* name, const vec2& v) override;
		void setVec3(const char* name, const vec3& v) override;
		void setVec4(const char* name, const vec4& v) override;
		void setMat3(const char* name, const mat3& m) override;
		void setMat4(const char* name, const mat4& m) override;
		void setTexture(const char* name, StringId sid) override;

		inline unsigned int getLoc(const char* name);

		OpenGLId getId() const { return _id; }
	
	private:
		OpenGLId _id;
	};
}

#endif// ATTA_GRAPHICS_SYSTEM_RENDERER_APIS_OPENGL_OPENGL_SHADER_GROUP_H
