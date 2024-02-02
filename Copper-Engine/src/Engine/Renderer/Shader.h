#pragma once
// Source is in Platform/<API>/<API>Shader.cpp

#include "Engine/Core/Core.h"
#include "cupch.h"

#include <GLM/mat4x4.hpp>

namespace Copper {

	// Higher level, platform indenpendant wrapper for the VertexArray
	class Shader {

	public:
		Shader() = default;
		Shader(const std::string& vertexPath, const std::string& fragmentPath);

		void Bind() const;
		void Unbind() const;

		void LoadInt(const std::string& name, int32 value) const;
		void LoadUInt(const std::string& name, uint32 value) const;
		void LoadFloat(const std::string& name, float value) const;
		
		void LoadVec2(const std::string& name, const Vector2& vec) const;
		void LoadVec3(const std::string& name, const Vector3& vec) const;
		void LoadVec4(const std::string& name, const Vector4& vec) const;

		void LoadColor(const std::string& name, const Color& col) const;

		void LoadMat3(const std::string& name, const Matrix3& mat) const;
		void LoadMat4(const std::string& name, const Matrix4& mat) const;

	private:
		uint32 m_id;
		uint32 m_vertex;
		uint32 m_fragment;

	};

}