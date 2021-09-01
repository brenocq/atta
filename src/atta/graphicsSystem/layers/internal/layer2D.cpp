//--------------------------------------------------
// Atta Graphics System
// layer2D.cpp
// Date: 2021-09-01
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/graphicsSystem/layers/internal/layer2D.h>
#include <imgui_internal.h>
#include <glad/glad.h>

namespace atta
{
	unsigned int VAO;
	unsigned int shaderProgram;

	Layer2D::Layer2D():
		Layer(StringId("2D"))
	{
	}

	void Layer2D::onAttach()
	{
		//-----------------------------------//
		//------------- Shader --------------//
		//-----------------------------------//

		//---------- vertex shader ----------//
		const char *vertexShaderSource = "#version 330 core\n"
			"layout (location = 0) in vec3 aPos;\n"
			"layout (location = 1) in vec3 aColor;\n"
			"uniform vec2 delta;"
			"out vec3 color;"
			"void main()\n"
			"{\n"
			"   gl_Position = vec4(aPos.x + delta.x, aPos.y + delta.y, aPos.z, 1.0);\n"
			"	color = aColor;"
			"}\0";

		unsigned int vertexShader;
		vertexShader = glCreateShader(GL_VERTEX_SHADER);
		// Link source and compile
		glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
		glCompileShader(vertexShader);

		// Check errors
		int  success;
		char infoLog[512];
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
		if(!success)
		{
			glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
		}

		//---------- fragment shader ----------//
		const char *fragmentShaderSource = "#version 330 core\n"
			"in vec3 color;"
			"out vec4 FragColor;\n"
			"void main()\n"
			"{\n"
			"   FragColor = vec4(color, 1.0f);\n"
			"}\0";

		unsigned int fragmentShader;
		fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
		glCompileShader(fragmentShader);

		// Check errors
		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
		if(!success)
		{
			glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
		}


		//----- Shader program -----//
		shaderProgram = glCreateProgram();

		glAttachShader(shaderProgram, vertexShader);
		glAttachShader(shaderProgram, fragmentShader);
		glLinkProgram(shaderProgram);

		// Check errors
		glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
		if(!success) {
			glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::LINK_FAILED\n" << infoLog << std::endl;
		}

		glUseProgram(shaderProgram);
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);

		//-----------------------------------//
		//--------------- VAO ---------------//
		//-----------------------------------//
		
		//---------- Vertex Array Object ----------//
		glGenVertexArrays(1, &VAO); 

		//----- Buffers -----//
		float vertices[] = {
			// positions         // colors
			 0.5f, -0.5f, 0.0f,  0.6f, 0.3f, 0.1f,   // bottom right
			-0.5f, -0.5f, 0.0f,  0.2f, 0.6f, 0.2f,   // bottom left
			 0.0f,  0.5f, 0.0f,  0.1f, 0.3f, 0.6f    // top
		};
		unsigned int indices[] = {  // note that we start from 0!
			0, 1, 2,
		};  

		unsigned int VBO;
		// Create buffer
		glGenBuffers(1, &VBO);

		glBindVertexArray(VAO);
		// Bind buffer as array buffer
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		// Send data to buffer (memory for a lot of readings and few writes)
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		unsigned int EBO;
		glGenBuffers(1, &EBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		//----- Data layout -----//
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3*sizeof(float)));
		glEnableVertexAttribArray(1);

		// Unbind buffer and VAO
		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}

	void Layer2D::onDetach()
	{
	}

	void Layer2D::onRender()
	{
		glUseProgram(shaderProgram);

		int vertexDeltaLocation = glGetUniformLocation(shaderProgram, "delta");
		glUniform2f(vertexDeltaLocation, 0.5f, 0.5f);

		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}

	void Layer2D::onUIRender()
	{
		// XXX Only testing
		ImGui::Begin("Layer 2D window");
        ImGui::Button("Hello!");
        ImGui::Button("Testiiing!");
		ImGui::End();
	}
}
