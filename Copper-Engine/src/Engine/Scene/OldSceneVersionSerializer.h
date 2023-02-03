#pragma once

#include "Engine/Core/Core.h"

#include "Engine/Scene/Scene.h"

namespace YAML {

	class Node;
	class Emitter;

}

namespace Copper {

	struct ScriptField;
	class ScriptComponent;

	class OldSceneVersionSerializer {

	public:
		static void SerializeVersion_Beta1_0_0(YAML::Emitter& out, Scene* scene);
		static bool DeserializeVersion_Beta1_0_0(const YAML::Node& data, Scene* scene);

	private:
		template<typename T> static void WriteField_Beta1_0_0(YAML::Emitter& out, ScriptField& field, ScriptComponent* scriptInstance);
		template<typename T> static void ReadField_Beta1_0_0(YAML::Node& value, ScriptField& field, ScriptComponent* scriptInstance);

	};

}