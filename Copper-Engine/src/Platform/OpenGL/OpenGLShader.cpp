#include "cupch.h"
#include "Engine/Renderer/Shader.h"

#include "Engine/Utilities/FileUtils.h"

#include <glad/glad.h>
#include <GLM/gtc/type_ptr.hpp>

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

	void Shader::LoadInt(std::string name, int value) { glUniform1i(glGetUniformLocation(ID, name.c_str()), value); }
	void Shader::LoadUInt(std::string name, uint32_t value) { glUniform1ui(glGetUniformLocation(ID, name.c_str()), value); }
	void Shader::LoadFloat(std::string name, float value) { glUniform1f(glGetUniformLocation(ID, name.c_str()), value); }
	void Shader::LoadMat4(std::string name, glm::mat4 mat) { glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(mat)); }
	void Shader::LoadVec3(std::string name, Vector3 vec) { glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, glm::value_ptr((glm::vec3) vec)); }

	void Shader::Bind() { glUseProgram(ID); }
	void Shader::Unbind() { glUseProgram(0); }

	Shader::~Shader() { glDeleteProgram(ID); }

}