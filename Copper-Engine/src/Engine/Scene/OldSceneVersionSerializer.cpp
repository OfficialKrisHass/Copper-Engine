#include "cupch.h"
#include "OldSceneVersionSerializer.h"

#include "Engine/Scene/CopperECS.h"

#include "Engine/Components/Camera.h"
#include "Engine/Components/MeshRenderer.h"
#include "Engine/Components/ScriptComponent.h"
#include "Engine/Components/Light.h"

#include "Engine/Scripting/ScriptingCore.h"
#include "Engine/Scripting/Field.h"

#include <fstream>
#include <yaml-cpp/yaml.h>

namespace Copper {

	

	void OldSceneVersionSerializer::SerializeVersion_Beta1_0_0(YAML::Emitter& out, Scene* scene) {

		out << YAML::BeginMap;

		out << YAML::Key << "Version" << YAML::Value << YAML::Flow;
		out << "Beta" << 1 << 0 << 0;

		out << YAML::Key << "Scene" << YAML::Value << scene->name;
		out << YAML::Key << "Entities" << YAML::Value << YAML::BeginMap;

		for (InternalEntity* entity : EntityView(scene)) {

			out << YAML::Key << entity->id;
			out << YAML::Value << YAML::BeginMap;

			//Name
			out << YAML::Key << "Name" << YAML::Value << entity->name;

			//Transform
			out << YAML::Key << "Transform";
			out << YAML::Value << YAML::BeginMap;

			out << YAML::Key << "Position" << YAML::Value << entity->transform->position;
			out << YAML::Key << "Rotation" << YAML::Value << entity->transform->rotation;
			out << YAML::Key << "Scale" << YAML::Value << entity->transform->scale;

			out << YAML::Key << "Parent" << YAML::Value;
			if (entity->transform->parent) out << entity->transform->parent->entity->id;
			else out << -1;

			out << YAML::Key << "Children" << YAML::Value << YAML::BeginSeq;

			for (int i = 0; i < entity->transform->numOfChildren; i++) { out << entity->transform->children[i]; }

			out << YAML::EndSeq;

			out << YAML::EndMap;

			//Components
			out << YAML::Key << "Components";
			out << YAML::Value << YAML::BeginMap;

			if (Light* light = entity->GetComponent<Light>()) {

				out << YAML::Key << "Light" << YAML::Value << YAML::BeginMap;

				out << YAML::Key << "Color" << YAML::Value << light->color;
				out << YAML::Key << "Intensity" << YAML::Value << light->intensity;

				out << YAML::EndMap;

			}
			if (Camera* cam = entity->GetComponent<Camera>()) {

				out << YAML::Key << "Camera" << YAML::Value << YAML::BeginMap;

				out << YAML::Key << "Fov" << YAML::Value << cam->fov;
				out << YAML::Key << "Near Plane" << YAML::Value << cam->nearPlane;
				out << YAML::Key << "Far Plane" << YAML::Value << cam->farPlane;

				out << YAML::EndMap;

			}
			if (MeshRenderer* renderer = entity->GetComponent<MeshRenderer>()) {

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
			if (ScriptComponent* script = entity->GetComponent<ScriptComponent>()) {

				out << YAML::Key << "Script Component" << YAML::Value << YAML::BeginMap; //Script Component

				out << YAML::Key << "Name" << YAML::Value << script->name;

				out << YAML::Key << "Fields" << YAML::Value << YAML::BeginMap; //Fields
				for (ScriptField& field : Scripting::GetScriptFields(script->name)) {

					switch (field.type) {

						case ScriptField::Type::Int: { WriteField_Beta1_0_0<int>(out, field, script); break; }
						case ScriptField::Type::UInt: { WriteField_Beta1_0_0<unsigned int>(out, field, script); break; }
						case ScriptField::Type::Float: { WriteField_Beta1_0_0<float>(out, field, script); break; }
						case ScriptField::Type::Entity: { WriteField_Beta1_0_0<InternalEntity*>(out, field, script); break; }

					}

				}
				out << YAML::EndMap; //Fields

				out << YAML::EndMap; //Script Component

			}
			out << YAML::EndMap; //Components

			out << YAML::EndMap; //Object

		}

		out << YAML::EndMap;
		out << YAML::EndMap;
		std::ofstream file(scene->path);
		file << out.c_str();

	}
	bool OldSceneVersionSerializer::DeserializeVersion_Beta1_0_0(const YAML::Node& data, Scene* scene) {

		scene->name = data["Scene"].as<std::string>();

		YAML::Node entities = data["Entities"];
		for (YAML::const_iterator it = entities.begin(); it != entities.end(); ++it) {

			YAML::Node entity = it->second;

			std::string name = entity["Name"].as<std::string>();
			int32_t id = it->first.as<int>();
			InternalEntity* deserialized = scene->CreateEntityFromID(id, Vector3::zero, Vector3::zero, Vector3::one, name);

			deserialized->transform->position = entity["Transform"]["Position"].as<Vector3>();
			deserialized->transform->rotation = entity["Transform"]["Rotation"].as<Vector3>();
			deserialized->transform->scale = entity["Transform"]["Scale"].as<Vector3>();

			uint32_t parentID = entity["Transform"]["Parent"].as<unsigned int>();

			if (parentID == invalidID) deserialized->transform->parent = nullptr;
			else if (id > parentID) {

				Transform* parent = scene->GetEntityFromID(parentID)->transform;
				deserialized->transform->parent = parent;
				for (int i = 0; i < parent->numOfChildren; i++) {

					if (parent->children[i] == id) {

						deserialized->transform->parentChildIndex = i;
						break;

					}

				}

			}
			else if (id < parentID) {

				//This little maneuver is gonna cost us 51 miliseconds
				Entity savedDeserialized = deserialized;
				InternalEntity* parent = scene->CreateEntityFromID(parentID, Vector3::zero, Vector3::zero, Vector3::one, "Empty Parent");
				deserialized = savedDeserialized;

				deserialized->transform->parent = parent->transform;
				deserialized->transform->parentChildIndex = 0;

			}

			for (int i = 0; i < entity["Transform"]["Children"].size(); i++) {

				deserialized->transform->children.push_back(entity["Transform"]["Children"][i].as<int>());
				deserialized->transform->numOfChildren++;

			}

			YAML::Node components = entity["Components"];

			for (YAML::const_iterator it = components.begin(); it != components.end(); ++it) {

				YAML::Node component = it->second;
				std::string cName = it->first.as<std::string>();

				if (cName == "Light") {

					Light* l = deserialized->AddComponent<Light>();

					l->color = component["Color"].as<Color>();
					l->intensity = component["Intensity"].as<float>();

				}
				if (cName == "Camera") {

					Camera* cam = deserialized->AddComponent<Camera>();

					cam->fov = component["Fov"].as<float>();
					cam->nearPlane = component["Near Plane"].as<float>();
					cam->farPlane = component["Far Plane"].as<float>();

				}
				if (cName == "Mesh Renderer") {

					MeshRenderer* renderer = deserialized->AddComponent<MeshRenderer>();

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

					ScriptComponent* s = deserialized->AddComponent<ScriptComponent>();
					s->Init(deserialized->id, component["Name"].as<std::string>());

					YAML::Node fields = component["Fields"];
					for (ScriptField& field : Scripting::GetScriptFields(s->name)) {

						switch (field.type) {

							case ScriptField::Type::Int: { ReadField_Beta1_0_0<int>(fields[field.name]["Value"], field, s); break; }
							case ScriptField::Type::UInt: { ReadField_Beta1_0_0<unsigned int>(fields[field.name]["Value"], field, s); break; }
							case ScriptField::Type::Float: { ReadField_Beta1_0_0<float>(fields[field.name]["Value"], field, s); break; }
							case ScriptField::Type::Entity: { ReadField_Beta1_0_0<InternalEntity*>(fields[field.name]["Value"], field, s); break; }

						}

					}

				}

			}

		}

		return true;

	}

	template<typename T> void OldSceneVersionSerializer::WriteField_Beta1_0_0(YAML::Emitter& out, ScriptField& field, ScriptComponent* scriptInstance) {

		T tmp;
		scriptInstance->GetFieldValue(field, &tmp);

		out << YAML::Key << field.name << YAML::Value << YAML::BeginMap; //Field

		out << YAML::Key << "Type" << YAML::Value << (int) field.type;
		out << YAML::Key << "Value" << YAML::Value << tmp;

		out << YAML::EndMap; //Field

	}
	template<typename T> void OldSceneVersionSerializer::ReadField_Beta1_0_0(YAML::Node& value, ScriptField& field, ScriptComponent* scriptInstance) {

		T tmp = value.as<T>();
		scriptInstance->SetFieldValue(field, &tmp);

	}

}