//--------------------------------------------------
// Atta Graphics System
// shader.h
// Date: 2021-09-09
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_GRAPHICS_SYSTEM_SHADER_H
#define ATTA_GRAPHICS_SYSTEM_SHADER_H
#include <atta/core/stringId.h>
#include <atta/graphicsSystem/shaderUniform.h>

namespace atta
{
	class Shader
	{
	public:
		struct CreateInfo
		{
			fs::path filepath;
		};

		Shader(const CreateInfo& info);
		virtual ~Shader() = default;

		virtual void recompile() = 0;

	protected:
		fs::path _filepath;
		std::unordered_map<std::string, ShaderUniform> _uniforms;
	};
}

#endif// ATTA_GRAPHICS_SYSTEM_SHADER_H
