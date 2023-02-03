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

		void LoadInt(const std::string& name, int value) const;
		void LoadUInt(const std::string& name, uint32_t value) const;
		void LoadFloat(const std::string& name, float value) const;
		
		void LoadMat4(const std::string& name, glm::mat4 mat) const;
		void LoadVec3(const std::string& name, Vector3 vec) const;

	private:
		uint32_t ID;
		uint32_t vertex;
		uint32_t fragment;

		void CreateShader(const std::string& path, int type, uint32_t& id);
		void CheckShader(uint32_t id, int type);
		void CheckShader(uint32_t id);

	};

}