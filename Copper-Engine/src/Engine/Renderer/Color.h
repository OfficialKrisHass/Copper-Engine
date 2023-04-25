#pragma once

#include <CMath/Vectors.h>

#include <yaml-cpp/yaml.h>

namespace Copper {

	struct Color {

		Color(float all = 0.0f, float a = 1.0f) : r(all), g(all), b(all), a(a) {}
		Color(float r, float g, float b, float a = 1.0f) : r(r), g(g), b(b), a(a) {}

		float r;
		float g;
		float b;
		float a;

		static Color red;
		static Color green;
		static Color blue;
		static Color white;
		static Color black;
		static Color transparent;

		operator CMath::Vector3() const { return CMath::Vector3(r, g, b); }
		operator CMath::Vector4() const { return CMath::Vector4(r, g, b, a); }

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

}