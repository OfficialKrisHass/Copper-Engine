#include "cupch.h"

#include "Engine/Core/Engine.h"

#include "Engine/Renderer/Renderer.h"
#include "Engine/Renderer/Mesh.h"

#include "Engine/Components/Camera.h"
#include "Engine/Components/MeshRenderer.h"
#include "Engine/Components/Light.h"

#include <fstream>
#include <yaml-cpp/yaml.h>

#include <CopperECS/CopperECS.h>

namespace Copper {

	int cCounter = 0;

	Object Scene::CreateObject(Vector3 position, Vector3 rotation, Vector3 scale, std::string name) {

		Object obj = registry.CreateObject(this, position, rotation, scale, name);

		return obj;

	}
	Object Scene::CreateObject(std::string name) { return CreateObject(Vector3::zero, Vector3::zero, Vector3::one, name); }

	void Scene::StartRuntime() {

		runtimeRunning = true;

	}
	void Scene::Update(bool render) {

		Renderer::ClearColor(0.18f, 0.18f, 0.18f);

		cam = nullptr;

		for (Object& o : SceneView<>(this)) {

			o.transform->Update();

			if (o.HasComponent<Light>()) light = o.GetComponent<Light>();
			if (o.HasComponent<Camera>()) cam = o.GetComponent<Camera>();
			if (o.HasComponent<MeshRenderer>()) {

				MeshRenderer* renderer = o.GetComponent<MeshRenderer>();
				for (Mesh mesh : renderer->meshes) {

					Renderer::AddMesh(&mesh, o.transform);

				}

			}
			if (runtimeRunning && o.HasComponent<ScriptComponent>()) {

				if (!runtimeStarted) o.GetComponent<ScriptComponent>()->InvokeCreate(); 

				o.GetComponent<ScriptComponent>()->InvokeUpdate();

			}

		}

		if (runtimeRunning && !runtimeStarted) runtimeStarted = true;

		if (cam && render) {

			cam->Resize(GetWindowSize());
			Render(cam);

		}

	}
	void Scene::Render(Camera* camera) {

		Renderer::RenderFrame(camera, light);

	}

	void Scene::Serialize(std::filesystem::path path) {

		this->path = path;
		this->name = path.stem().string();

		YAML::Emitter out;
		out << YAML::BeginMap;

		out << YAML::Key << "Scene" << YAML::Value << name;
		out << YAML::Key << "Entities" << YAML::Value << YAML::BeginMap;

		for(Object& o : SceneView<>(this)) {
			
			out << YAML::Key << o.id;
			out << YAML::Value << YAML::BeginMap;

			//Name
			out << YAML::Key << "Name" << YAML::Value << o.tag->name;

			//Transform
			out << YAML::Key << "Transform";
			out << YAML::Value << YAML::BeginMap;

			out << YAML::Key << "Position" << YAML::Value << o.transform->position;
			out << YAML::Key << "Rotation" << YAML::Value << o.transform->rotation;
			out << YAML::Key << "Scale" << YAML::Value << o.transform->scale;

			out << YAML::Key << "Parent" << YAML::Value;
			if (o.transform->parent) out << o.transform->parent->object->GetID();
			else out << -1;

			out << YAML::Key << "Children" << YAML::Value << YAML::BeginSeq;

			for (int i = 0; i < o.transform->numOfChildren; i++) { out << o.transform->children[i]; }

			out << YAML::EndSeq;

			out << YAML::EndMap;
			
			//Components
			out << YAML::Key << "Components";
			out << YAML::Value << YAML::BeginMap;

			for (Light* light : o.GetComponents<Light>()) {

				out << YAML::Key << "Light" << YAML::Value << YAML::BeginMap;

				out << YAML::Key << "Color" << YAML::Value << light->color;
				out << YAML::Key << "Intensity" << YAML::Value << light->intensity;

				out << YAML::EndMap;
				
			}
			for (Camera* cam : o.GetComponents<Camera>()) {

				out << YAML::Key << "Camera" << YAML::Value << YAML::BeginMap;

				out << YAML::Key << "Fov" << YAML::Value << cam->fov;
				out << YAML::Key << "Near Plane" << YAML::Value << cam->nearPlane;
				out << YAML::Key << "Far Plane" << YAML::Value << cam->farPlane;

				out << YAML::EndMap;
				
			}
			for (MeshRenderer* renderer : o.GetComponents<MeshRenderer>()) {

				out << YAML::Key << "Mesh Renderer" << YAML::Value << YAML::BeginMap;

				for (Mesh mesh : renderer->meshes) {

					out << YAML::Key << "Mesh" << YAML::Value << YAML::BeginMap;

					out << YAML::Key << "Data" << YAML::Value << YAML::BeginMap;
					for (int i = 0; i < mesh.vertices.size(); i++) {

						out << YAML::Key << "Vertex" << YAML::Value << YAML::BeginMap;

						out << YAML::Key << "Position" << YAML::Value << mesh.vertices[i];
						out << YAML::Key << "Normal" << YAML::Value << mesh.normals[i];
						out << YAML::Key << "Color" << YAML::Value << mesh.colors[i];

						out << YAML::EndMap;

					}
					out << YAML::EndMap;

					out << YAML::Key << "Indices" << YAML::Value << YAML::BeginSeq;
					for (int i = 0; i < mesh.indices.size(); i++) {

						out << mesh.indices[i];

					}
					out << YAML::EndSeq;

					out << YAML::EndMap;

				}

				//out << YAML::EndMap;
				out << YAML::EndMap;
				
			}
			for (ScriptComponent* script : o.GetComponents<ScriptComponent>()) {

				out << YAML::Key << "Script Component" << YAML::Value << YAML::BeginMap;

				out << YAML::Key << "Name" << YAML::Value << script->name;

				out << YAML::EndMap;

			}
			out << YAML::EndMap; //Components

			out << YAML::EndMap; //Object
			
		}

		out << YAML::EndMap;
		out << YAML::EndMap;
		std::ofstream file(path);
		file << out.c_str();
		
	}
	bool Scene::Deserialize(std::filesystem::path path) {

		this->path = path;

		YAML::Node data;
		try { data = YAML::LoadFile(path.string()); } catch(YAML::ParserException e) {

			LogError("Failed to Read .scene file. {0}\n    {1}", path.string(), e.what());
			return false;
			
		}

		name = data["Scene"].as<std::string>();

		YAML::Node entities = data["Entities"];
		for (YAML::const_iterator it = entities.begin(); it != entities.end(); ++it) {

			YAML::Node entity = it->second;
			
			std::string name = entity["Name"].as<std::string>();
			int32_t id = it->first.as<int>();
			Object deserialized = registry.CreateObjectFromID(id, this, Vector3::zero, Vector3::zero, Vector3::one, name);

			deserialized.transform->position = entity["Transform"]["Position"].as<Vector3>();
			deserialized.transform->rotation = entity["Transform"]["Rotation"].as<Vector3>();
			deserialized.transform->scale = entity["Transform"]["Scale"].as<Vector3>();

			int32_t parentID = entity["Transform"]["Parent"].as<int>();

			if (parentID == -1)  deserialized.transform->parent = nullptr;
			else if (id > parentID) deserialized.transform->parent = registry.GetObjectFromID(parentID).transform;
			else if (id < parentID) deserialized.transform->parent = registry.CreateObjectFromID(parentID, this, Vector3::zero, Vector3::zero, Vector3::one, "Empty Parent").transform;

			for (int i = 0; i < entity["Transform"]["Children"].size(); i++) {

				deserialized.transform->children.push_back(entity["Transform"]["Children"][i].as<int>());
				deserialized.transform->numOfChildren++;

			}

			YAML::Node components = entity["Components"];

			for (YAML::const_iterator it = components.begin(); it != components.end(); ++it) {

				YAML::Node component = it->second;
				std::string cName = it->first.as<std::string>();

				if (cName == "Light") {

					Light* l = deserialized.AddComponent<Light>();

					l->color = component["Color"].as<Color>();
					l->intensity = component["Intensity"].as<float>();

				}
				if (cName == "Camera") {

					Camera* cam = deserialized.AddComponent<Camera>();

					cam->fov = component["Fov"].as<float>();
					cam->nearPlane = component["Near Plane"].as<float>();
					cam->farPlane = component["Far Plane"].as<float>();

				}
				if (cName == "Mesh Renderer") {

					MeshRenderer* renderer = deserialized.AddComponent<MeshRenderer>();

					for (YAML::const_iterator it = component.begin(); it != component.end(); ++it) {

						YAML::Node mesh = it->second;
						Mesh m;

						YAML::Node meshData = mesh["Data"];
						for (YAML::const_iterator it = meshData.begin(); it != meshData.end(); ++it) {

							YAML::Node vertex = it->second;

							m.vertices.push_back(vertex["Position"].as<Vector3>());
							m.normals.push_back(vertex["Normal"].as<Vector3>());
							m.colors.push_back(vertex["Color"].as<Color>());

						}

						YAML::Node indices = mesh["Indices"];
						for (int i = 0; i < indices.size(); i++) {

							m.indices.push_back(indices[i].as<uint32_t>());

						}

						renderer->meshes.push_back(m);

					}

				}
				if (cName == "Script Component") {

					ScriptComponent* s = deserialized.AddComponent<ScriptComponent>();

					s->Init(deserialized.id, component["Name"].as<std::string>());

				}

			}

		}

		return true;
		
	}

}