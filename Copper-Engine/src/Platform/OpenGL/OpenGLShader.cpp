#include "cupch.h"
#include "Engine/Renderer/Shader.h"

#include "Engine/Utilities/FileUtils.h"

#include <glad/glad.h>
#include <GLM/gtc/type_ptr.hpp>

namespace Copper {

	int32 success;
	char infoLog[512];

	Shader::Shader(const std::string& vertexPath, const std::string& fragmentPath) {

		CreateShader(vertexPath, GL_VERTEX_SHADER, m_vertex);
		CreateShader(fragmentPath, GL_FRAGMENT_SHADER, m_fragment);

		m_id = glCreateProgram();

		glAttachShader(m_id, m_vertex);
		glAttachShader(m_id, m_fragment);
		glLinkProgram(m_id);

		CheckShader(m_id);

		glDeleteShader(m_vertex);
		glDeleteShader(m_fragment);

	}

	void Shader::CreateShader(const std::string& path, uint32 type, uint32& id) {

		id = glCreateShader(type);

		const std::string& source = Utilities::ReadFile(path);
		const char* src = source.c_str();

		glShaderSource(id, 1, &src, NULL);
		glCompileShader(id);

		CheckShader(id, type);

	}

	void Shader::CheckShader(uint32 id, uint32 type) {

		glGetShaderiv(id, GL_COMPILE_STATUS, &success);

		if (!success) {

			glGetShaderInfoLog(id, 512, NULL, infoLog);
			switch (type) {

			case GL_VERTEX_SHADER: LogError("Vertex Shader Compilation Failed!\n\n{0}", infoLog); break;
			case GL_FRAGMENT_SHADER: LogError("Fragment Shader Compilation Failed!\n\n{0}", infoLog); break;

			}

		}

	}

	void Shader::CheckShader(uint32 id) {

		glGetProgramiv(id, GL_LINK_STATUS, &success);

		if (!success) {

			glGetProgramInfoLog(id, 512, NULL, infoLog);

			LogError("Shader Linking Failed!\n\n{0}", infoLog);

		}

	}

	void Shader::LoadInt(const std::string& name, int32 value) const { glUniform1i(glGetUniformLocation(m_id, name.c_str()), value); }
	void Shader::LoadUInt(const std::string& name, uint32 value) const { glUniform1ui(glGetUniformLocation(m_id, name.c_str()), value); }
	void Shader::LoadFloat(const std::string& name, float value) const { glUniform1f(glGetUniformLocation(m_id, name.c_str()), value); }
	void Shader::LoadMat4(const std::string& name, const Matrix4& mat) const { glUniformMatrix4fv(glGetUniformLocation(m_id, name.c_str()), 1, GL_FALSE, &(mat.cols[0].x)); }
	void Shader::LoadMat4TEST(const std::string& name, const glm::mat4& mat) const { glUniformMatrix4fv(glGetUniformLocation(m_id, name.c_str()), 1, GL_FALSE, glm::value_ptr(mat)); }
	void Shader::LoadVec3(const std::string& name, const Vector3& vec) const { glUniform3fv(glGetUniformLocation(m_id, name.c_str()), 1, &(vec.x)); }

	void Shader::Bind() const { glUseProgram(m_id); }
	void Shader::Unbind() const { glUseProgram(0); }

}