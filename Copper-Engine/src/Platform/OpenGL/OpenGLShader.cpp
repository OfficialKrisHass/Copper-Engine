#include "cupch.h"
#include "Engine/Renderer/Shader.h"

#include "Engine/Utilities/FileUtils.h"

#include <glad/glad.h>

namespace Copper {

	int success;
	char infoLog[512];

	Shader::Shader(std::string vertexPath, std::string fragmentPath) {

		CreateShader(vertexPath, GL_VERTEX_SHADER, vertex);
		CreateShader(fragmentPath, GL_FRAGMENT_SHADER, fragment);

		ID = glCreateProgram();

		glAttachShader(ID, vertex);
		glAttachShader(ID, fragment);
		glLinkProgram(ID);

		CheckShader(ID);

		glDeleteShader(vertex);
		glDeleteShader(fragment);

	}

	void Shader::CreateShader(std::string path, int type, uint32_t& id) {

		id = glCreateShader(type);

		std::string source = Utilities::ReadFile(path);
		const char* src = source.c_str();

		glShaderSource(id, 1, &src, NULL);
		glCompileShader(id);

		CheckShader(id, type);

	}

	void Shader::CheckShader(uint32_t id, int type) {

		glGetShaderiv(id, GL_COMPILE_STATUS, &success);

		if (!success) {

			glGetShaderInfoLog(id, 512, NULL, infoLog);

			switch (type) {

			case GL_VERTEX_SHADER: LogError("Vertex Shader Compilation Failed!\n\n{0}", infoLog); break;
			case GL_FRAGMENT_SHADER: LogError("Fragment Shader Compilation Failed!\n\n{0}", infoLog);

			}

		}

	}

	void Shader::CheckShader(uint32_t id) {

		glGetProgramiv(id, GL_LINK_STATUS, &success);

		if (!success) {

			glGetProgramInfoLog(id, 512, NULL, infoLog);

			LogError("Shader Linking Failed!\n\n{0}", infoLog);

		}

	}

	void Shader::Bind() { glUseProgram(ID); }
	void Shader::Unbind() { glUseProgram(0); }

	Shader::~Shader() { glDeleteProgram(ID); }

}