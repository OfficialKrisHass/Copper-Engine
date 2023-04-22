#pragma once

#include "Engine/Math/Math.h"

#include <yaml-cpp/yaml.h>

namespace Copper {

	struct Color {

		Color() = default;
		Color(float all, float alpha = 1.0f) : r(all), g(all), b(all), a(alpha) {}
		Color(float r, float g, float b, float alpha = 1.0f) : r(r), g(g), b(b), a(alpha) {}

		float r = 0.0f;
		float g = 0.0f;
		float b = 0.0f;
		float a = 1.0f;

		operator Vector3() const { return Vector3(r, g, b); }
		operator Vector4() const { return Vector4(r, g, b, a); }

		static Color red;
		static Color green;
		static Color blue;
		static Color white;
		static Color black;
		static Color transparent;

		bool operator==(const Color& other) const {

			if (r != other.r || g != other.g || b != other.b || a != other.a) return false;

			return true;

		}
		bool operator!=(const Color& other) const {

			return !(*this == other);

		}

	};

	inline std::ostream& operator<<(std::ostream& os, const Color& col) {

		return os << "R: " << col.r << ", G: " << col.g << ", B: " << col.b << ", A: " << col.a;

	}
	inline YAML::Emitter& operator<<(YAML::Emitter& out, const Color& col) {

		out << YAML::Flow;
		out << YAML::BeginSeq << col.r << col.g << col.b << col.a << YAML::EndSeq;
		return out;

	}

}

namespace YAML {

	using namespace Copper;

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