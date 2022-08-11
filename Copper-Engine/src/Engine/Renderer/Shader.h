#pragma once

#include "Engine/Core/Core.h"
#include "cupch.h"

namespace Copper {

	class Shader {

	public:
		Shader() = default;
		Shader(std::string vertexPath, std::string fragmentPath);
		~Shader();

		void Bind();
		void Unbind();

	private:
		uint32_t ID;
		uint32_t vertex;
		uint32_t fragment;

		void CreateShader(std::string path, int type, uint32_t& id);
		void CheckShader(uint32_t id, int type);
		void CheckShader(uint32_t id);

	};

}