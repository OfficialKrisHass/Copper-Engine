#include "cupch.h"
#include "Engine/Renderer/Shader.h"

#include "Engine/Utilities/FileUtils.h"

#include <glad/glad.h>

namespace Copper {

	int32 success = 0;
	char infoLog[2048];

	static constexpr const char* ShaderTypeToString(uint32 type) {

		switch (type) {

		case GL_VERTEX_SHADER: return "Vertex Shader";
		case GL_FRAGMENT_SHADER: return "Fragment Shader";

		}

		LogError("Invalid shader type! {{}}", type);
		return nullptr;

	}

	static uint32 CreateShader(const std::string& path, uint32 type);
	static void CheckShaderCompile(uint32 id, uint32 type);
	static void CheckShaderLink(uint32 id);

	Shader::Shader(const std::string& vertexPath, const std::string& fragmentPath) {

		m_vertex = CreateShader(vertexPath, GL_VERTEX_SHADER);
		m_fragment = CreateShader(fragmentPath, GL_FRAGMENT_SHADER);

		m_id = glCreateProgram();

		glAttachShader(m_id, m_vertex);
		glAttachShader(m_id, m_fragment);
		glLinkProgram(m_id);

		CheckShaderLink(m_id);

		glDeleteShader(m_vertex);
		glDeleteShader(m_fragment);

	}

	uint32 CreateShader(const std::string& path, uint32 type) {

		uint32 id = glCreateShader(type);

		const std::string& source = Utilities::ReadFile(path);
		const char* src = source.c_str();

		glShaderSource(id, 1, &src, NULL);
		glCompileShader(id);

		CheckShaderCompile(id, type);
		return id;

	}

	void CheckShaderCompile(uint32 id, uint32 type) {

		glGetShaderiv(id, GL_COMPILE_STATUS, &success);

		if (success) return;

		glGetShaderInfoLog(id, 2048, NULL, infoLog);
		LogError("{} Compilation Failed!\n\t{}", ShaderTypeToString(type), infoLog);

	}
	void CheckShaderLink(uint32 id) {

		glGetProgramiv(id, GL_LINK_STATUS, &success);

		if (success) return;

		glGetProgramInfoLog(id, 512, NULL, infoLog);
		LogError("Shader Linking Failed!\n\t{}", infoLog);

	}

	void Shader::LoadInt(const std::string& name, int32 value) const { glUniform1i(glGetUniformLocation(m_id, name.c_str()), value); }
	void Shader::LoadUInt(const std::string& name, uint32 value) const { glUniform1ui(glGetUniformLocation(m_id, name.c_str()), value); }
	void Shader::LoadFloat(const std::string& name, float value) const { glUniform1f(glGetUniformLocation(m_id, name.c_str()), value); }

	void Shader::LoadVec2(const std::string& name, const Vector2& vec) const { glUniform2fv(glGetUniformLocation(m_id, name.c_str()), 1, &(vec.x)); }
	void Shader::LoadVec3(const std::string& name, const Vector3& vec) const { glUniform3fv(glGetUniformLocation(m_id, name.c_str()), 1, &(vec.x)); }
	void Shader::LoadVec4(const std::string& name, const Vector4& vec) const { glUniform4fv(glGetUniformLocation(m_id, name.c_str()), 1, &(vec.x)); }

	void Shader::LoadColor(const std::string& name, const Color& col) const { glUniform4fv(glGetUniformLocation(m_id, name.c_str()), 1, &(col.r)); }

	void Shader::LoadMat3(const std::string& name, const Matrix3& mat) const { glUniformMatrix3fv(glGetUniformLocation(m_id, name.c_str()), 1, GL_FALSE, &(mat.cols[0].x)); }
	void Shader::LoadMat4(const std::string& name, const Matrix4& mat) const { glUniformMatrix4fv(glGetUniformLocation(m_id, name.c_str()), 1, GL_FALSE, &(mat.cols[0].x)); }

	void Shader::Bind() const { glUseProgram(m_id); }
	void Shader::Unbind() const { glUseProgram(0); }

}