//--------------------------------------------------
// Atta Graphics System
// shaderUniform.h
// Date: 2021-09-09
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_GRAPHICS_SYSTEM_SHADER_UNIFORM_H
#define ATTA_GRAPHICS_SYSTEM_SHADER_UNIFORM_H

namespace atta
{
	class ShaderUniform final
	{
	public:
		enum class Type
		{
			NONE = 0, 
			BOOL, 
			INT, UINT, 
			FLOAT, 
			VEC2, VEC3, VEC4, 
			IVEC2, IVEC3, IVEC4,
			MAT3, MAT4 
		};
		ShaderUniform(std::string name, Type type, uint32_t size, uint32_t offset);

		const std::string& getName() const { return _name; }
		Type getType() const { return _type; }
		uint32_t getSize() const { return _size; }
		uint32_t getOffset() const { return _offset; }

	private:
		std::string _name;
		Type _type;
		uint32_t _size;
		uint32_t _offset;
	};
}

#endif// ATTA_GRAPHICS_SYSTEM_SHADER_UNIFORM_H
