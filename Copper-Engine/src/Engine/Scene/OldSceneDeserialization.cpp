#include "cupch.h"
#include "OldSceneDeserialization.h"

#include "Engine/Scene/CopperECS.h"

#include "Engine/Components/Camera.h"
#include "Engine/Components/MeshRenderer.h"
#include "Engine/Components/ScriptComponent.h"
#include "Engine/Components/Light.h"

#include "Engine/Scripting/ScriptingCore.h"
#include "Engine/Scripting/Field.h"

#include <fstream>
#include <yaml-cpp/yaml.h>
#include "Engine/YAMLOverloads/Everything.h"

namespace Copper {

	bool OldSceneDeserialization::DeserializeVersion_Beta1_0_0(const YAML::Node& data, Scene* scene) {

		scene->name = data["Scene"].as<std::string>();

		YAML::Node entities = data["Entities"];
		for (YAML::const_iterator it = entities.begin(); it != entities.end(); ++it) {

			YAML::Node entity = it->second;

			std::string name = entity["Name"].as<std::string>();
			uint32_t id = it->first.as<uint32_t>();
			InternalEntity* deserialized = scene->CreateEntityFromID(id, Vector3::zero, Vector3::zero, Vector3::one, name, false);

			deserialized->transform->position = entity["Transform"]["Position"].as<Vector3>();
			deserialized->transform->rotation = entity["Transform"]["Rotation"].as<Quaternion>();
			deserialized->transform->scale = entity["Transform"]["Scale"].as<Vector3>();

			uint32_t parentID = entity["Transform"]["Parent"].as<uint32_t>();

			if (parentID == INVALID_ENTITY_ID) deserialized->transform->parent = nullptr;
			else if (id > parentID) {

				Transform* parent = scene->GetEntityFromID(parentID)->transform;
				deserialized->transform->parent = parent;

			}
			else if (id < parentID) {

				//This little maneuver is gonna cost us 51 miliseconds
				Entity savedDeserialized = deserialized;
				InternalEntity* parent = scene->CreateEntityFromID(parentID, Vector3::zero, Vector3::zero, Vector3::one, "Empty Parent");
				deserialized = savedDeserialized;

				deserialized->transform->parent = parent->transform;

			}

			for (int i = 0; i < entity["Transform"]["Children"].size(); i++) {

				deserialized->transform->children.push_back(entity["Transform"]["Children"][i].as<int>());

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
					s->Init(component["Name"].as<std::string>());

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

	template<typename T> void OldSceneDeserialization::ReadField_Beta1_0_0(const YAML::Node& value, ScriptField& field, ScriptComponent* scriptInstance) {

		T tmp = value.as<T>();
		scriptInstance->SetFieldValue(field, &tmp);

	}

}