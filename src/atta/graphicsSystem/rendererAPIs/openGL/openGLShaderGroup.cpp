//--------------------------------------------------
// Atta Graphics System
// openGLShaderGroup.cpp
// Date: 2021-09-09
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/graphicsSystem/rendererAPIs/openGL/openGLShaderGroup.h>
#include <atta/graphicsSystem/rendererAPIs/openGL/openGLShader.h>

namespace atta
{
	OpenGLShaderGroup::OpenGLShaderGroup(const ShaderGroup::CreateInfo& info):
		ShaderGroup(info), _id(0)
	{
		// Create shaders (they are compiled at creation)
		for(auto shaderPath : info.shaderPaths)
		{
			Shader::CreateInfo info {};
			info.filepath = shaderPath;
			_shaders.push_back(std::static_pointer_cast<Shader>(std::make_shared<OpenGLShader>(info)));
		}

		recompile();
	}

	OpenGLShaderGroup::~OpenGLShaderGroup()
	{
		if(_id > 0)
		{
			glDeleteProgram(_id);
			_id = 0;
		}
	}

	void OpenGLShaderGroup::recompile()
	{
		if(_id > 0)
		{
			glDeleteProgram(_id);
			_id = 0;
		}

		// Recompile shaders
		for(auto& shader : _shaders)
			shader->recompile();

		_id = glCreateProgram();

		// Attach shaders
		for(auto shader : _shaders)
			glAttachShader(_id, std::static_pointer_cast<OpenGLShader>(shader)->getId());

		// Link shaders
		glLinkProgram(_id);

		int success;
		char infoLog[512];
		glGetProgramiv(_id, GL_LINK_STATUS, &success);
		if(!success)
		{
			glGetProgramInfoLog(_id, 512, NULL, infoLog);
			std::vector<std::string> shaderPaths;
			for(auto shader : _shaders)
				shaderPaths.push_back(fs::absolute(shader->getFilepath()).string());
			ASSERT(false, "Failed to link shader group [w]$0[]:\n[*w]Shaders:[w]$1\n[*w]Error:[w]\n$2", _debugName, shaderPaths, infoLog);
		}

		// Detach and delete shaders
		//glUseProgram(_id);
		for(auto shader : _shaders)
		{
			std::shared_ptr<OpenGLShader> s = std::static_pointer_cast<OpenGLShader>(shader);
			//glDetachShader(_id, s->getId());
			s->deleteShader();
		}
		//glUseProgram(0);
	}

	void OpenGLShaderGroup::bind()
	{
		glUseProgram(_id);
	}
}
