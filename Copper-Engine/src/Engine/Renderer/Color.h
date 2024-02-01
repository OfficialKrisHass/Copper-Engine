#pragma once

#include <CMath/Vectors.h>

namespace Copper {

	// Represantation of RGBA as four 0 to 1 floating point numbers
	struct Color {

		typedef unsigned char uint8;
		typedef unsigned int uint32;

		Color() = default;
		Color(float all, float a = 1.0f) : r(all), g(all), b(all), a(a) {}
		Color(float r, float g, float b, float a = 1.0f) : r(r), g(g), b(b), a(a) {}

		Color(const CMath::Vector3& vec) : r(vec.x), g(vec.y), b(vec.z), a(1.0f) {}
		Color(const CMath::Vector4& vec) : r(vec.x), g(vec.y), b(vec.z), a(vec.w) {}

		// Data

		float r = 0.0f;
		float g = 0.0f;
		float b = 0.0f;
		float a = 0.0f;

		// Static variables

		static const Color red;
		static const Color green;
		static const Color blue;
						
		static const Color white;
		static const Color black;
		
		static const Color transparent;

		// Assignement

		inline Color& operator=(const Color& other) {

			this->r = other.r;
			this->g = other.g;
			this->b = other.b;
			this->a = other.a;
			return *this;

		}

		inline Color& operator=(const CMath::Vector3& other) {

			this->r = other.x;
			this->g = other.y;
			this->b = other.z;
			this->a = 1.0f;
			return *this;

		}
		inline Color& operator=(const CMath::Vector4& other) {

			this->r = other.x;
			this->g = other.y;
			this->b = other.z;
			this->a = other.w;
			return *this;

		}

		// Conversions

		inline operator CMath::Vector3() const { return CMath::Vector3(r, g, b); }
		inline operator CMath::Vector4() const { return CMath::Vector4(r, g, b, a); }

		// Comparison

		bool operator==(const Color& other) const { return r == other.r && g == other.g && b == other.b && a == other.a; }
		bool operator!=(const Color& other) const { return !(*this == other); }

	};

	inline std::ostream& operator<(std::ostream& os, const Color& col) { return os << "R: " << col.r << ", G: " << col.g << ", B: " << col.b << ", A: " << col.a; }

	const Color inline Color::red = Color(1.0f, 0.0f, 0.0f);
	const Color inline Color::green = Color(0.0f, 1.0f, 0.0f);
	const Color inline Color::blue = Color(0.0f, 0.0f, 1.0f);
	
	const Color inline Color::white = Color(1.0f);
	const Color inline Color::black = Color(0.0f);
	
	const Color inline Color::transparent = Color(1.0f, 0.0f);

}