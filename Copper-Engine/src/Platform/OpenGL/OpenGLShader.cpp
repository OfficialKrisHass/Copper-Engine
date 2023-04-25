#include "cupch.h"
#include "Engine/Renderer/Shader.h"

#include "Engine/Utilities/FileUtils.h"

#include <glad/glad.h>
#include <GLM/gtc/type_ptr.hpp>

namespace Copper {

	int success;
	char infoLog[512];

	Shader::Shader(const std::string& vertexPath, const std::string& fragmentPath) {

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

	void Shader::CreateShader(const std::string& path, int type, uint32_t& id) {

		id = glCreateShader(type);

		const std::string& source = Utilities::ReadFile(path);
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
			case GL_FRAGMENT_SHADER: LogError("Fragment Shader Compilation Failed!\n\n{0}", infoLog); break;

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

	void Shader::LoadInt(const std::string& name, int value) const { glUniform1i(glGetUniformLocation(ID, name.c_str()), value); }
	void Shader::LoadUInt(const std::string& name, uint32_t value) const { glUniform1ui(glGetUniformLocation(ID, name.c_str()), value); }
	void Shader::LoadFloat(const std::string& name, float value) const { glUniform1f(glGetUniformLocation(ID, name.c_str()), value); }
	void Shader::LoadMat4(const std::string& name, const Matrix4& mat) const { glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &(mat.cols[0].x)); }
	void Shader::LoadMat4TEST(const std::string& name, const glm::mat4& mat) const { glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(mat)); }
	void Shader::LoadVec3(const std::string& name, const Vector3& vec) const { glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &(vec.x)); }

	void Shader::Bind() const { glUseProgram(ID); }
	void Shader::Unbind() const { glUseProgram(0); }

}