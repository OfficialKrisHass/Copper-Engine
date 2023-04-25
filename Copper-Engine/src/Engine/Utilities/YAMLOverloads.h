#pragma once

#include "Engine/Core/Core.h"

namespace YAML { class Emitter; }

namespace Copper {

	template<typename T>
	YAML::Emitter& operator<<(YAML::Emitter& out, const CMath::vec2<T>& vec) {

		out << YAML::Flow << YAML::BeginSeq << vec.x << vec.y << YAML::EndSeq;
		return out;

	}
	template<typename T>
	YAML::Emitter& operator<<(YAML::Emitter& out, const CMath::vec3<T>& vec) {

		out << YAML::Flow << YAML::BeginSeq << vec.x << vec.y << vec.z << YAML::EndSeq;
		return out;

	}
	template<typename T>
	YAML::Emitter& operator<<(YAML::Emitter& out, const CMath::vec4<T>& vec) {

		out << YAML::Flow << YAML::BeginSeq << vec.x << vec.y << vec.z << vec.w << YAML::EndSeq;
		return out;

	}

	YAML::Emitter& operator<<(YAML::Emitter& out, const Color& col) {

		out << YAML::Flow << YAML::BeginSeq << col.r << col.g << col.b << col.a << YAML::EndSeq;
		return out;

	}

	inline YAML::Emitter& operator<<(YAML::Emitter& out, const Color& col);

}

namespace YAML {

	using namespace Copper;

	template<typename T> struct convert<CMath::vec2<T>> {

		static Node encode(const CMath::vec2<T>& vec) {

			Node node;
			node.push_back(vec.x);
			node.push_back(vec.y);
			node.SetStyle(EmitterStyle::Flow);

			return node;

		}
		static bool decode(const Node& node, CMath::vec2<T>& vec) {

			if (!node.IsSequence() || node.size() != 2) return false;

			vec.x = node[0].as<T>();
			vec.y = node[1].as<T>();

			return true;

		}

	};
	template<typename T> struct convert<CMath::vec3<T>> {

		static Node encode(const CMath::vec3<T>& vec) {

			Node node;
			node.push_back(vec.x);
			node.push_back(vec.y);
			node.push_back(vec.z);
			node.SetStyle(EmitterStyle::Flow);

			return node;

		}
		static bool decode(const Node& node, CMath::vec3<T>& vec) {

			if (!node.IsSequence() || node.size() != 3) return false;

			vec.x = node[0].as<T>();
			vec.y = node[1].as<T>();
			vec.z = node[2].as<T>();

			return true;

		}

	};
	template<typename T> struct convert<CMath::vec4<T>> {

		static Node encode(const CMath::vec4<T>& vec) {

			Node node;
			node.push_back(vec.x);
			node.push_back(vec.y);
			node.push_back(vec.z);
			node.push_back(vec.w);
			node.SetStyle(EmitterStyle::Flow);

			return node;

		}
		static bool decode(const Node& node, CMath::vec4<T>& vec) {

			if (!node.IsSequence() || node.size() != 4) return false;

			vec.x = node[0].as<T>();
			vec.y = node[1].as<T>();
			vec.z = node[2].as<T>();
			vec.w = node[3].as<T>();

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

			if (!node.IsSequence() || node.size() != 4) return false;

			col.r = node[0].as<float>();
			col.g = node[1].as<float>();
			col.b = node[2].as<float>();
			col.a = node[3].as<float>();

			return true;

		}

	};

}