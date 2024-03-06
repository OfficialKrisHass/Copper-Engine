#pragma once

#include "Engine/Core/Core.h"

#include "Engine/AssetStorage/AssetPtr.h"

#include "Engine/YAMLOverloads/UUID.h"

#include <yaml-cpp/yaml.h>

namespace Copper {

	template<typename AssetType> inline YAML::Emitter& operator<<(YAML::Emitter& out, const AssetPtr<AssetType>& asset) {

		out << asset.AssetUUID();
		return out;

	}

}

namespace YAML {

	using namespace Copper;

	template<typename AssetType> struct convert<AssetPtr<AssetType>> {

		static Node encode(const AssetPtr<AssetType>& asset) {

			Node node;
			node.push_back(asset.AssetUUID().str());

			return node;

		}
		static bool decode(const Node& node, AssetPtr<AssetType>& asset) {

			if (!node.IsScalar()) return false;

			asset = node.as<UUID>();

			return true;

		}

	};

}