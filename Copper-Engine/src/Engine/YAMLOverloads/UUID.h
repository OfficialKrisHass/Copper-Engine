#pragma once

#include "Engine/Core/Core.h"
#include "Engine/Core/UUID.h"

#include <yaml-cpp/yaml.h>

namespace Copper {

	inline YAML::Emitter& operator<<(YAML::Emitter& out, const UUID& uuid) {

		out << uuid.str();
		return out;

	}

}

namespace YAML {

	using namespace Copper;

	template<> struct convert<UUID> {

		static Node encode(const UUID& uuid) {

			Node node;
			node.push_back(uuid.str());

			return node;

		}
		static bool decode(const Node& node, UUID& uuid) {

			if (!node.IsScalar()) return false;

			std::string data = node.as<std::string>();
			if (data == "")
				uuid = UUID("");
			else
				uuid = UUID::fromStrFactory(data);

			return true;

		}

	};

}