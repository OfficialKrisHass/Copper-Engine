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

		CUP_FUNCTION_START();

		m_vertex = CreateShader(vertexPath, GL_VERTEX_SHADER);
		m_fragment = CreateShader(fragmentPath, GL_FRAGMENT_SHADER);

		m_id = glCreateProgram();

		glAttachShader(m_id, m_vertex);
		glAttachShader(m_id, m_fragment);
		glLinkProgram(m_id);

		CheckShaderLink(m_id);

		glDeleteShader(m_vertex);
		glDeleteShader(m_fragment);

		CUP_SCOPE_END();

	}

	uint32 CreateShader(const std::string& path, uint32 type) {

		CUP_FUNCTION_START();

		uint32 id = glCreateShader(type);

		const std::string& source = Utilities::ReadFile(path);
		const char* src = source.c_str();

		glShaderSource(id, 1, &src, NULL);
		glCompileShader(id);

		CheckShaderCompile(id, type);

		CUP_SCOPE_END();
		return id;

	}

	void CheckShaderCompile(uint32 id, uint32 type) {

		CUP_FUNCTION_START();

		glGetShaderiv(id, GL_COMPILE_STATUS, &success);

		if (success) {

			CUP_SCOPE_END();
			return;

		}

		glGetShaderInfoLog(id, 2048, NULL, infoLog);
		LogError("{} Compilation Failed!\n\t{}", ShaderTypeToString(type), infoLog);

		CUP_SCOPE_END();

	}
	void CheckShaderLink(uint32 id) {

		CUP_FUNCTION_START();

		glGetProgramiv(id, GL_LINK_STATUS, &success);

		if (success) {

			CUP_SCOPE_END();
			return;

		}

		glGetProgramInfoLog(id, 512, NULL, infoLog);
		LogError("Shader Linking Failed!\n\t{}", infoLog);

		CUP_SCOPE_END();

	}

	uint32 Shader::UniformLocation(const std::string& name) const { return (uint32) glGetUniformLocation(m_id, name.c_str()); }

	void Shader::LoadInt(uint32 location, int32 value) const { glUniform1i(location, value); }
	void Shader::LoadUInt(uint32 location, uint32 value) const { glUniform1ui(location, value); }
	void Shader::LoadFloat(uint32 location, float value) const { glUniform1f(location, value); }

	void Shader::LoadVec2(uint32 location, const Vector2& vec) const { glUniform2fv(location, 1, &(vec.x)); }
	void Shader::LoadVec3(uint32 location, const Vector3& vec) const { glUniform3fv(location, 1, &(vec.x)); }
	void Shader::LoadVec4(uint32 location, const Vector4& vec) const { glUniform4fv(location, 1, &(vec.x)); }

	void Shader::LoadColor(uint32 location, const Color& col) const { glUniform4fv(location, 1, &(col.r)); }

	void Shader::LoadMat3(uint32 location, const Matrix3& mat) const { glUniformMatrix3fv(location, 1, GL_FALSE, &(mat.cols[0].x)); }
	void Shader::LoadMat4(uint32 location, const Matrix4& mat) const { glUniformMatrix4fv(location, 1, GL_FALSE, &(mat.cols[0].x)); }

	void Shader::Bind() const { glUseProgram(m_id); }
	void Shader::Unbind() const { glUseProgram(0); }

}