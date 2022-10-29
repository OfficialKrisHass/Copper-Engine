#include "cupch.h"
#include "Scene.h"

#include "Engine/Renderer/Renderer.h"
#include "Engine/Renderer/Mesh.h"

#include "Engine/Scene/Object.h"

#include "Engine/Scene/Components/Camera.h"
#include "Engine/Scene/Components/MeshRenderer.h"

#include <fstream>
#include <yaml-cpp/yaml.h>

#include "../../../../Copper-Editor/src/Viewport/SceneCamera.h"

namespace YAML {

	using namespace Copper;

	template<> struct convert<Vector2> {

		static Node encode(const Vector2& vec) {

			Node node;
			
			node.push_back(vec.x);
			node.push_back(vec.y);
			node.SetStyle(EmitterStyle::Flow);

			return node;
			
		}
		
		static bool decode(const Node& node, Vector2& vec) {

			if(!node.IsSequence() || node.size() != 2) return false;

			vec.x = node[0].as<float>();
			vec.y = node[1].as<float>();

			return true;
			
		}
		
	};
	template<> struct convert<Vector3> {

		static Node encode(const Vector3& vec) {

			Node node;
			
			node.push_back(vec.x);
			node.push_back(vec.y);
			node.push_back(vec.z);
			node.SetStyle(EmitterStyle::Flow);

			return node;
			
		}
		
		static bool decode(const Node& node, Vector3& vec) {

			if(!node.IsSequence() || node.size() != 3) return false;

			vec.x = node[0].as<float>();
			vec.y = node[1].as<float>();
			vec.z = node[2].as<float>();

			return true;
			
		}
		
	};
	template<> struct convert<Vector4> {

		static Node encode(const Vector4& vec) {

			Node node;
			
			node.push_back(vec.x);
			node.push_back(vec.y);
			node.push_back(vec.z);
			node.push_back(vec.w);
			node.SetStyle(EmitterStyle::Flow);

			return node;
			
		}
		
		static bool decode(const Node& node, Vector4& vec) {

			if(!node.IsSequence() || node.size() != 4) return false;

			vec.x = node[0].as<float>();
			vec.y = node[1].as<float>();
			vec.z = node[2].as<float>();
			vec.w = node[3].as<float>();

			return true;
			
		}
		
	};
	template<> struct convert<Color> {

		static Node encode(const Color& col) {

			Node node;
			
			node.push_back(col.r);
			node.push_back(col.g);
			node.push_back(col.b);
			node.push_back(col.a);
			node.SetStyle(EmitterStyle::Flow);

			return node;
			
		}
		
		static bool decode(const Node& node, Color& col) {

			if(!node.IsSequence() || node.size() != 4) return false;

			col.r = node[0].as<float>();
			col.g = node[1].as<float>();
			col.b = node[2].as<float>();
			col.a = node[3].as<float>();

			return true;
			
		}
		
	};
	
}

namespace Copper {

	YAML::Emitter& operator<<(YAML::Emitter& out, const Vector2& vec) {

		out << YAML::Flow;
		out << YAML::BeginSeq << vec.x << vec.y << YAML::EndSeq;
		return out;
		
	}
	YAML::Emitter& operator<<(YAML::Emitter& out, const Vector3& vec) {

		out << YAML::Flow;
		out << YAML::BeginSeq << vec.x << vec.y << vec.z << YAML::EndSeq;
		return out;
		
	}
	YAML::Emitter& operator<<(YAML::Emitter& out, const Vector4& vec) {

		out << YAML::Flow;
		out << YAML::BeginSeq << vec.x << vec.y << vec.z << vec.w << YAML::EndSeq;
		return out;
		
	}
	YAML::Emitter& operator<<(YAML::Emitter& out, const Color& col) {

		out << YAML::Flow;
		out << YAML::BeginSeq << col.r << col.g << col.b << col.a << YAML::EndSeq;
		return out;
		
	}

	int ECS::cCounter = 0;

	Object Scene::CreateObject(Vector3 position, Vector3 rotation, Vector3 scale, std::string name) {

		Object obj(registry.CreateEntity(), this);

		obj.name = registry.AddComponent<Name>(obj.id);
		obj.name->name = name;

		obj.transform = registry.AddComponent<Transform>(obj.id);
		obj.transform->position = position;
		obj.transform->rotation = rotation;
		obj.transform->scale = scale;

		return obj;

	}
	Object Scene::CreateObject(Transform* transform, std::string name) {

		Object obj(registry.CreateEntity(), this);

		obj.name = registry.AddComponent<Name>(obj.id);
		obj.name->name = name;

		obj.transform = registry.AddComponent<Transform>(obj.id);
		obj.transform->position = transform->position;
		obj.transform->rotation = transform->rotation;
		obj.transform->scale = transform->scale;

		return obj;

	}
	Object Scene::CreateObject(std::string name) {

		Object obj(registry.CreateEntity(), this);

		obj.name = registry.AddComponent<Name>(obj.id);
		obj.name->name = name;

		obj.transform = registry.AddComponent<Transform>(obj.id);
		return obj;

	}

	void Scene::DestroyObject(Object obj) {

		registry.DestroyEntitiy(obj.id);

	}

	void Scene::Update() {

		Renderer::ClearColor(0.18f, 0.18f, 0.18f);

		cam->Update();

		Light* directional = nullptr;

		for(Object o : SceneView<Light>(*this)) {

			directional = o.GetComponent<Light>();
			
		}

		for (Object o : SceneView<MeshRenderer>(*this)) {

			MeshRenderer* renderer = o.GetComponent<MeshRenderer>();

			for (Mesh mesh : renderer->meshes) {

				Renderer::AddMesh(&mesh, o.transform);

			}

		}

		Renderer::Render(cam, directional);

	}

	void Scene::Serialize(std::filesystem::path path) {

		this->path = path;
		this->name = path.stem().string();

		YAML::Emitter out;
		out << YAML::BeginMap;

		out << YAML::Key << "Scene" << YAML::Value << name;
		out << YAML::Key << "Entities" << YAML::Value << YAML::BeginMap;

		for(Object o : SceneView<>(*this)) {
			
			out << YAML::Key << o.id;
			out << YAML::Value << YAML::BeginMap;

			//Name
			out << YAML::Key << "Name" << YAML::Value << o.name->name;

			//Transform
			out << YAML::Key << "Transform";
			out << YAML::Value << YAML::BeginMap;

			out << YAML::Key << "Position" << YAML::Value << o.transform->position;
			out << YAML::Key << "Rotation" << YAML::Value << o.transform->rotation;
			out << YAML::Key << "Scale" << YAML::Value << o.transform->scale;

			out << YAML::EndMap;
			
			//Components
			out << YAML::Key << "Components";
			out << YAML::Value << YAML::BeginMap;

			if(o.HasComponent<Camera>()) {

				Camera* cam = o.GetComponent<Camera>();

				out << YAML::Key << "Camera" << YAML::Value << YAML::BeginMap;

				out << YAML::Key << "Fov" << YAML::Value << cam->fov;
				out << YAML::Key << "Near Plane" << YAML::Value << cam->nearPlane;
				out << YAML::Key << "Far Plane" << YAML::Value << cam->farPlane;

				out << YAML::EndMap;
				
			}
			if(o.HasComponent<MeshRenderer>()) {

				MeshRenderer* renderer = o.GetComponent<MeshRenderer>();

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

				out << YAML::EndMap;
				
			}
			if(o.HasComponent<Light>()) {

				Light* light = o.GetComponent<Light>();

				out << YAML::Key << "Light" << YAML::Value << YAML::BeginMap;
				out << YAML::Key << "Color" << YAML::Value << "None";

				out << YAML::EndMap;
				
			}

			out << YAML::EndMap;
			out << YAML::EndMap;
			
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
			Object deserialized = CreateObject(name);

			deserialized.transform->position = entity["Transform"]["Position"].as<Vector3>();
			deserialized.transform->rotation = entity["Transform"]["Rotation"].as<Vector3>();
			deserialized.transform->scale = entity["Transform"]["Scale"].as<Vector3>();

			YAML::Node components = entity["Components"];
			
			YAML::Node camera = components["Camera"];
			if(camera) {

				Camera* cam = deserialized.AddComponent<Camera>();

				cam->fov = camera["Fov"].as<float>();
				cam->nearPlane = camera["Near Plane"].as<float>();
				cam->farPlane = camera["Far Plane"].as<float>();
				
			}

			YAML::Node meshRenderer = components["Mesh Renderer"];
			if(meshRenderer) {

				MeshRenderer* renderer = deserialized.AddComponent<MeshRenderer>();

				for (YAML::const_iterator it = meshRenderer.begin(); it != meshRenderer.end(); ++it) {

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

			YAML::Node light = components["Light"];
			if(light) {

				Light* l = deserialized.AddComponent<Light>();

				l->color = Color::White();
				l->intensity = 1.0f;
				
			}
			
		}

		return true;
		
	}

}