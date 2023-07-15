#pragma once

#define CMATH_PARENT_NAMESPACE Copper
#include <CMath/CMath.h>

#include <yaml-cpp/yaml.h>

namespace Copper {

	template<typename T> inline YAML::Emitter& operator<<(YAML::Emitter& out, const CMath::vec<2, T>& vec) {

		out << YAML::Flow << YAML::BeginSeq << vec.x << vec.y << YAML::EndSeq;
		return out;

	}
	template<typename T> inline YAML::Emitter& operator<<(YAML::Emitter& out, const CMath::vec<3, T>& vec) {

		out << YAML::Flow << YAML::BeginSeq << vec.x << vec.y << vec.z << YAML::EndSeq;
		return out;

	}
	template<typename T> inline YAML::Emitter& operator<<(YAML::Emitter& out, const CMath::vec<4, T>& vec) {

		out << YAML::Flow << YAML::BeginSeq << vec.x << vec.y << vec.z << vec.w << YAML::EndSeq;
		return out;

	}

}

namespace YAML {

	template<typename T> struct convert<Copper::CMath::vec<2, T>> {

		static Node encode(const Copper::CMath::vec<2, T>& vec) {

			Node node;
			node.push_back(vec.x);
			node.push_back(vec.y);
			node.SetStyle(EmitterStyle::Flow);

			return node;

		}
		static bool decode(const Node& node, Copper::CMath::vec<2, T>& vec) {

			if (!node.IsSequence() || node.size() != 2) return false;

			vec.x = node[0].as<T>();
			vec.y = node[1].as<T>();

			return true;

		}

	};
	template<typename T> struct convert<Copper::CMath::vec<3, T>> {

		static Node encode(const Copper::CMath::vec<3, T>& vec) {

			Node node;
			node.push_back(vec.x);
			node.push_back(vec.y);
			node.push_back(vec.z);
			node.SetStyle(EmitterStyle::Flow);

			return node;

		}
		static bool decode(const Node& node, Copper::CMath::vec<3, T>& vec) {

			if (!node.IsSequence() || node.size() != 3) return false;

			vec.x = node[0].as<T>();
			vec.y = node[1].as<T>();
			vec.z = node[2].as<T>();

			return true;

		}

	};
	template<typename T> struct convert<Copper::CMath::vec<4, T>> {

		static Node encode(const Copper::CMath::vec<4, T>& vec) {

			Node node;
			node.push_back(vec.x);
			node.push_back(vec.y);
			node.push_back(vec.z);
			node.push_back(vec.w);
			node.SetStyle(EmitterStyle::Flow);

			return node;

		}
		static bool decode(const Node& node, Copper::CMath::vec<4, T>& vec) {

			if (!node.IsSequence() || node.size() != 4) return false;

			vec.x = node[0].as<T>();
			vec.y = node[1].as<T>();
			vec.z = node[2].as<T>();
			vec.w = node[3].as<T>();

			return true;

		}

	};

}