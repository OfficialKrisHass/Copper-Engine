#pragma once

#include "Engine/Core/Core.h"
#include "cupch.h"

#include <GLM/mat4x4.hpp>

namespace Copper {

	class Shader {

	public:
		Shader() = default;
		Shader(const std::string& vertexPath, const std::string& fragmentPath);

		void Bind() const;
		void Unbind() const;

		void LoadInt(const std::string& name, int32 value) const;
		void LoadUInt(const std::string& name, uint32 value) const;
		void LoadFloat(const std::string& name, float value) const;
		
		void LoadMat4(const std::string& name, const Matrix4& mat) const;
		void LoadMat4TEST(const std::string& name, const glm::mat4& mat) const;
		void LoadVec3(const std::string& name, const Vector3& vec) const;

	private:
		uint32 m_id;
		uint32 m_vertex;
		uint32 m_fragment;

		void CreateShader(const std::string& path, uint32 type, uint32& id);
		void CheckShader(uint32 id, uint32 type);
		void CheckShader(uint32 id);

	};

}