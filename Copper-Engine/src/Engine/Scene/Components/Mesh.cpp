#include "cupch.h"
#include "Mesh.h"

#include "Engine/Renderer/Renderer.h"
#include "Engine/Renderer/Buffer.h"

#include <fstream>
#include <yaml-cpp/yaml.h>

namespace Copper {

	void Mesh::Regenerate() {

		vao = new VertexArray();
		VertexBuffer* vbo = new VertexBuffer(vertices, { {"Position", ElementType::Float3}, {"Color", ElementType::Float3}, {"Normal", ElementType::Float3} });
		IndexBuffer* ibo = new IndexBuffer(indices);

		vao->SetVertexBuffer(vbo);
		vao->SetIndexBuffer(ibo);

		vbo->Unbind();
		vao->Unbind();

	}

	void Mesh::Update() {

		Renderer::GetShader()->LoadVec3("material.color", material.color);
		
	}
	
	void Mesh::GetMat() {
	    
	    YAML::Node data = YAML::LoadFile("assets/SerializationTest/Test.mat");

		std::string name = data["Name"].as<std::string>();
		std::string surname = data["Surname"].as<std::string>();
		int age = data["Age"].as<int>();

		Log("{0} {1} is {2} years old", name, surname, age);
	    
	}

	void Mesh::Test() {

		YAML::Emitter out;

		out << YAML::BeginMap;
		out << YAML::Key << "Name" << YAML::Value << "Roger";
		out << YAML::Key << "Surname" << YAML::Value << "DICK";
		out << YAML::Key << "Age" << YAML::Value << 25;
		out << YAML::EndMap;

		std::ofstream fout("assets/SerializationTest/Test.mat");
		fout << out.c_str();
		
	}

}