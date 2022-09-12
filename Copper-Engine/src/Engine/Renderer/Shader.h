#pragma once

#include "Engine/Core/Core.h"
#include "cupch.h"

#include <GLM/mat4x4.hpp>

namespace Copper {

	class Shader {

	public:
		Shader() = default;
		Shader(std::string vertexPath, std::string fragmentPath);
		~Shader();

		void Bind();
		void Unbind();

		void LoadMat4(std::string name, glm::mat4 mat);
		void LoadVec3(std::string name, Vector3 vec);

	private:
		uint32_t ID;
		uint32_t vertex;
		uint32_t fragment;

		void CreateShader(std::string path, int type, uint32_t& id);
		void CheckShader(uint32_t id, int type);
		void CheckShader(uint32_t id);

	};

}