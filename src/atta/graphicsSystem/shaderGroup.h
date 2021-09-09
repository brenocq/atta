//--------------------------------------------------
// Atta Graphics System
// shaderGroup.h
// Date: 2021-09-09
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_GRAPHICS_SYSTEM_SHADER_GROUP_H
#define ATTA_GRAPHICS_SYSTEM_SHADER_GROUP_H
#include <atta/graphicsSystem/shader.h>

namespace atta
{
	class ShaderGroup
	{
	public:
		struct CreateInfo
		{
			std::vector<fs::path> shaderPaths;

			StringId debugName = StringId("Unnamed Shader Group");
		};
		ShaderGroup(const CreateInfo& info);

		virtual ~ShaderGroup() = default;

		virtual void recompile() = 0;

		const std::vector<std::shared_ptr<Shader>>& getShaders() const { return _shaders; };

	protected:
		std::vector<std::shared_ptr<Shader>> _shaders;

		const StringId _debugName;
	};
}

#endif// ATTA_GRAPHICS_SYSTEM_SHADER_GROUP_H
