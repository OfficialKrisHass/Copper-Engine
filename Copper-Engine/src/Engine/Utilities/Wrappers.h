#pragma once

#include "Engine/Core/Core.h"

#include <GLM/vec2.hpp>
#include <GLM/vec3.hpp>
#include <GLM/vec4.hpp>

#include <yaml-cpp/yaml.h>

namespace Copper {

	struct UVector2I;
	struct Vector2I;

	struct Vector2 {

		Vector2() = default;
		Vector2(float all) : x(all), y(all) {}
		Vector2(float x, float y) : x(x), y(y) {}

		float x = 0.0f;
		float y = 0.0f;

		static Vector2 zero;
		static Vector2 one;
		static Vector2 minusOne;

		operator glm::vec2() const { return glm::vec2(x, y); }

		Vector2 operator+(const Vector2& other) const		{ return Vector2(x + other.x, y + other.y); }
		Vector2 operator+(const float other) const			{ return Vector2(x + other,   y + other); }
		Vector2 operator+(const glm::vec2& other) const		{ return Vector2(x + other.x, y + other.y); }
		Vector2 operator-(const Vector2& other) const		{ return Vector2(x - other.x, y - other.y); }
		Vector2 operator-(const float other) const			{ return Vector2(x - other,   y - other); }
		Vector2 operator-(const glm::vec2& other) const		{ return Vector2(x - other.x, y - other.y); }
		Vector2 operator*(const Vector2& other) const		{ return Vector2(x * other.x, y * other.y); }
		Vector2 operator*(const float other) const			{ return Vector2(x * other,   y * other); }
		Vector2 operator*(const glm::vec2& other) const		{ return Vector2(x * other.x, y * other.y); }
		Vector2 operator/(const Vector2& other) const		{ return Vector2(x / other.x, y / other.y); }
		Vector2 operator/(const float other) const			{ return Vector2(x / other,   y / other); }
		Vector2 operator/(const glm::vec2& other) const		{ return Vector2(x / other.x, y / other.y); }

		Vector2& operator=(const Vector2& other) {

			this->x = other.x;
			this->y = other.y;
			return *this;

		}
		Vector2& operator+=(const Vector2& other) {

			this->x += other.x;
			this->y += other.y;
			return *this;

		}
		Vector2& operator+=(const float& other) {

			this->x += other;
			this->y += other;
			return *this;

		}
		Vector2& operator-=(const Vector2& other) {

			this->x -= other.x;
			this->y -= other.y;
			return *this;

		}
		Vector2& operator-=(const float& other) {

			this->x -= other;
			this->y -= other;
			return *this;

		}
		Vector2& operator*=(const Vector2& other) {

			this->x *= other.x;
			this->y *= other.y;
			return *this;

		}
		Vector2& operator*=(const float& other) {

			this->x *= other;
			this->y *= other;
			return *this;

		}
		Vector2& operator/=(const Vector2& other) {

			this->x /= other.x;
			this->y /= other.y;
			return *this;

		}
		Vector2& operator/=(const float& other) {

			this->x /= other;
			this->y /= other;
			return *this;

		}

		Vector2& operator++() {

			++this->x;
			++this->y;
			return *this;

		}
		Vector2& operator--() {

			--this->x;
			--this->y;
			return *this;

		}

		bool operator==(const Vector2& other) const { return x == other.x && y == other.y; }
		bool operator!=(const Vector2& other) const { return !(*this == other); }

		Vector2 operator-() const { return Vector2(-x, -y); }

		operator UVector2I() const;
		operator Vector2I() const;

	};
	struct Vector3 {

		Vector3() = default;
		Vector3(glm::vec3 vec) : x(vec.x), y(vec.y), z(vec.z) {}
		Vector3(float all) : x(all), y(all), z(all) {}
		Vector3(float x, float y, float z) : x(x), y(y), z(z) {}

		float x = 0.0f;
		float y = 0.0f;
		float z = 0.0f;

		static Vector3 zero;
		static Vector3 one;
		static Vector3 minusOne;

		float Length() const {

			return sqrt(x * x + y * y + z * z);

		}

		operator glm::vec3() const { return glm::vec3(x, y, z); }

		Vector3 operator+(const Vector3& other) const		{ return Vector3(x + other.x, y + other.y, z + other.z); }
		Vector3 operator+(const float other) const			{ return Vector3(x + other,   y + other,   z + other); }
		Vector3 operator+(const glm::vec3& other) const		{ return Vector3(x + other.x, y + other.y, z + other.z); }
		Vector3 operator-(const Vector3& other) const		{ return Vector3(x - other.x, y - other.y, z - other.z); }
		Vector3 operator-(const float other) const			{ return Vector3(x - other,   y - other,   z - other); }
		Vector3 operator-(const glm::vec3& other) const		{ return Vector3(x - other.x, y - other.y, z - other.z); }
		Vector3 operator*(const Vector3& other) const		{ return Vector3(x * other.x, y * other.y, z * other.z); }
		Vector3 operator*(const float other) const			{ return Vector3(x * other,   y * other,   z * other); }
		Vector3 operator*(const glm::vec3& other) const		{ return Vector3(x * other.x, y * other.y, z * other.z); }
		Vector3 operator/(const Vector3& other) const		{ return Vector3(x / other.x, y / other.y, z / other.z); }
		Vector3 operator/(const float other) const			{ return Vector3(x / other,   y / other,   z / other); }
		Vector3 operator/(const glm::vec3& other) const		{ return Vector3(x / other.x, y / other.y, z / other.z); }

		Vector3& operator=(const Vector3& other) {

			this->x = other.x;
			this->y = other.y;
			this->z = other.z;
			return *this;

		}
		Vector3& operator=(const glm::vec3& other) {

			this->x = other.x;
			this->y = other.y;
			this->z = other.z;
			return *this;

		}
		Vector3& operator+=(const Vector3& other) {

			this->x += other.x;
			this->y += other.y;
			this->z += other.z;
			return *this;

		}
		Vector3& operator+=(const glm::vec3& other) {

			this->x += other.x;
			this->y += other.y;
			this->z += other.z;
			return *this;

		}
		Vector3& operator+=(const float& other) {

			this->x += other;
			this->y += other;
			this->z += other;
			return *this;

		}
		Vector3& operator-=(const Vector3& other) {

			this->x -= other.x;
			this->y -= other.y;
			this->z -= other.z;
			return *this;

		}
		Vector3& operator-=(const float& other) {

			this->x -= other;
			this->y -= other;
			this->z -= other;
			return *this;

		}
		Vector3& operator*=(const Vector3& other) {

			this->x *= other.x;
			this->y *= other.y;
			this->z *= other.z;
			return *this;

		}
		Vector3& operator*=(const float& other) {

			this->x *= other;
			this->y *= other;
			this->z *= other;
			return *this;

		}
		Vector3& operator/=(const Vector3& other) {

			this->x /= other.x;
			this->y /= other.y;
			this->z /= other.z;
			return *this;

		}
		Vector3& operator/=(const float& other) {

			this->x /= other;
			this->y /= other;
			this->z /= other;
			return *this;

		}

		Vector3& operator++() {

			++this->x;
			++this->y;
			++this->z;
			return *this;

		}
		Vector3& operator--() {

			--this->x;
			--this->y;
			--this->z;
			return *this;

		}

		bool operator==(const Vector3& other) const { return x == other.x && y == other.y && z == other.z; }
		bool operator!=(const Vector3& other) const { return !(*this == other); }

		Vector3 operator-() const { return Vector3(-x, -y, -z); }


	};
	struct Vector4 {

		Vector4() = default;
		Vector4(float all) : x(all), y(all), z(all), w(all) {}
		Vector4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}

		float x = 0.0f;
		float y = 0.0f;
		float z = 0.0f;
		float w = 0.0f;

		static Vector4 zero;
		static Vector4 one;
		static Vector4 minusOne;

		operator glm::vec4() const { return glm::vec4(x, y, z, w); }

		Vector4 operator+(const Vector4& other) const		{ return Vector4(x + other.x, y + other.y, z + other.z, w + other.w); }
		Vector4 operator+(const float other) const			{ return Vector4(x + other,   y + other,   z + other,   w + other); }
		Vector4 operator+(const glm::vec4& other) const		{ return Vector4(x + other.x, y + other.y, z + other.z, w + other.w); }
		Vector4 operator-(const Vector4& other) const		{ return Vector4(x - other.x, y - other.y, z - other.z, w - other.w); }
		Vector4 operator-(const float other) const			{ return Vector4(x - other,   y - other,   z - other,   w - other); }
		Vector4 operator-(const glm::vec4& other) const		{ return Vector4(x - other.x, y - other.y, z - other.z, w - other.w); }
		Vector4 operator*(const Vector4& other) const		{ return Vector4(x * other.x, y * other.y, z * other.z, w * other.w); }
		Vector4 operator*(const float other) const			{ return Vector4(x * other, y * other, z * other, w * other); }
		Vector4 operator*(const glm::vec4& other) const		{ return Vector4(x * other.x, y * other.y, z * other.z, w * other.w); }
		Vector4 operator/(const Vector4& other) const		{ return Vector4(x / other.x, y / other.y, z / other.z, w / other.w); }
		Vector4 operator/(const float other) const			{ return Vector4(x / other,   y / other,   z / other,   w / other); }
		Vector4 operator/(const glm::vec4& other) const		{ return Vector4(x / other.x, y / other.y, z / other.z, w / other.w); }

		Vector4& operator=(const Vector4& other) {

			this->x = other.x;
			this->y = other.y;
			this->z = other.z;
			this->w = other.w;
			return *this;

		}
		Vector4& operator+=(const Vector4& other) {

			this->x += other.x;
			this->y += other.y;
			this->z += other.z;
			this->w += other.w;
			return *this;

		}
		Vector4& operator+=(const float& other) {

			this->x += other;
			this->y += other;
			this->z += other;
			this->w += other;
			return *this;

		}
		Vector4& operator-=(const Vector4& other) {

			this->x -= other.x;
			this->y -= other.y;
			this->z -= other.z;
			this->w -= other.w;
			return *this;

		}
		Vector4& operator-=(const float& other) {

			this->x -= other;
			this->y -= other;
			this->z -= other;
			this->w -= other;
			return *this;

		}
		Vector4& operator*=(const Vector4& other) {

			this->x *= other.x;
			this->y *= other.y;
			this->z *= other.z;
			this->w *= other.w;
			return *this;

		}
		Vector4& operator*=(const float& other) {

			this->x *= other;
			this->y *= other;
			this->z *= other;
			this->w *= other;
			return *this;

		}
		Vector4& operator/=(const Vector4& other) {

			this->x /= other.x;
			this->y /= other.y;
			this->z /= other.z;
			this->w /= other.w;
			return *this;

		}
		Vector4& operator/=(const float& other) {

			this->x /= other;
			this->y /= other;
			this->z /= other;
			this->w /= other;
			return *this;

		}

		Vector4& operator++() {

			++this->x;
			++this->y;
			++this->z;
			++this->w;
			return *this;

		}
		Vector4& operator--() {

			--this->x;
			--this->y;
			--this->z;
			--this->w;
			return *this;

		}

		bool operator==(const Vector4& other) const { return x == other.x && y == other.y && z == other.z && w == other.w; }
		bool operator!=(const Vector4& other) const { return !(*this == other); }

		Vector4 operator-() const { return Vector4(-x, -y, -z, -w); }


	};

	struct Vector2I {

		Vector2I() = default;
		Vector2I(int32_t all) : x(all), y(all) {}
		Vector2I(int32_t x, int32_t y) : x(x), y(y) {}

		int32_t x = 0;
		int32_t y = 0;

		static Vector2I zero;
		static Vector2I one;
		static Vector2I minusOne;

		Vector2I operator+(const Vector2I& other) const { return Vector2I(x + other.x, y + other.y); }
		Vector2I operator+(const int32_t other) const { return Vector2I(x + other, y + other); }
		Vector2I operator+(const glm::uvec2& other) const { return Vector2I(x + other.x, y + other.y); }
		Vector2I operator-(const Vector2I& other) const { return Vector2I(x - other.x, y - other.y); }
		Vector2I operator-(const int32_t other) const { return Vector2I(x - other, y - other); }
		Vector2I operator-(const glm::uvec2& other) const { return Vector2I(x - other.x, y - other.y); }
		Vector2I operator*(const Vector2I& other) const { return Vector2I(x * other.x, y * other.y); }
		Vector2I operator*(const int32_t other) const { return Vector2I(x * other, y * other); }
		Vector2I operator*(const glm::uvec2& other) const { return Vector2I(x * other.x, y * other.y); }
		Vector2I operator/(const Vector2I& other) const { return Vector2I(x / other.x, y / other.y); }
		Vector2I operator/(const int32_t other) const { return Vector2I(x / other, y / other); }
		Vector2I operator/(const glm::uvec2& other) const { return Vector2I(x / other.x, y / other.y); }

		Vector2I& operator=(const Vector2I& other) {

			this->x = other.x;
			this->y = other.y;
			return *this;

		}
		Vector2I& operator+=(const Vector2I& other) {

			this->x += other.x;
			this->y += other.y;
			return *this;

		}
		Vector2I& operator+=(const int32_t& other) {

			this->x += other;
			this->y += other;
			return *this;

		}
		Vector2I& operator-=(const Vector2I& other) {

			this->x -= other.x;
			this->y -= other.y;
			return *this;

		}
		Vector2I& operator-=(const int32_t& other) {

			this->x -= other;
			this->y -= other;
			return *this;

		}
		Vector2I& operator*=(const Vector2I& other) {

			this->x *= other.x;
			this->y *= other.y;
			return *this;

		}
		Vector2I& operator*=(const int32_t& other) {

			this->x *= other;
			this->y *= other;
			return *this;

		}
		Vector2I& operator/=(const Vector2I& other) {

			this->x /= other.x;
			this->y /= other.y;
			return *this;

		}
		Vector2I& operator/=(const int32_t& other) {

			this->x /= other;
			this->y /= other;
			return *this;

		}

		Vector2I& operator++() {

			++this->x;
			++this->y;
			return *this;

		}
		Vector2I& operator--() {

			--this->x;
			--this->y;
			return *this;

		}

		Vector2I operator-() { return Vector2I(-x, -y); }

		bool operator==(const Vector2I& other) const { return x == other.x && y == other.y; }
		bool operator!=(const Vector2I& other) const { return !(*this == other); }

		operator Vector2() const;

	};
	struct UVector2I {

		UVector2I() = default;
		UVector2I(uint32_t all) : x(all), y(all) {}
		UVector2I(uint32_t x, uint32_t y) : x(x), y(y) {}

		uint32_t x = 0;
		uint32_t y = 0;

		static UVector2I zero;
		static UVector2I one;

		UVector2I operator+(const UVector2I& other) const		{ return UVector2I(x + other.x, y + other.y); }
		UVector2I operator+(const uint32_t other) const			{ return UVector2I(x + other,   y + other); }
		UVector2I operator+(const glm::uvec2& other) const		{ return UVector2I(x + other.x, y + other.y); }
		UVector2I operator-(const UVector2I& other) const		{ return UVector2I(x - other.x, y - other.y); }
		UVector2I operator-(const uint32_t other) const			{ return UVector2I(x - other,   y - other); }
		UVector2I operator-(const glm::uvec2& other) const		{ return UVector2I(x - other.x, y - other.y); }
		UVector2I operator*(const UVector2I& other) const		{ return UVector2I(x * other.x, y * other.y); }
		UVector2I operator*(const uint32_t other) const			{ return UVector2I(x * other,   y * other); }
		UVector2I operator*(const glm::uvec2& other) const		{ return UVector2I(x * other.x, y * other.y); }
		UVector2I operator/(const UVector2I& other) const		{ return UVector2I(x / other.x, y / other.y); }
		UVector2I operator/(const uint32_t other) const			{ return UVector2I(x / other,   y / other); }
		UVector2I operator/(const glm::uvec2& other) const		{ return UVector2I(x / other.x, y / other.y); }

		UVector2I& operator=(const UVector2I& other) {

			this->x = other.x;
			this->y = other.y;
			return *this;

		}
		UVector2I& operator+=(const UVector2I& other) {

			this->x += other.x;
			this->y += other.y;
			return *this;

		}
		UVector2I& operator+=(const uint32_t& other) {

			this->x += other;
			this->y += other;
			return *this;

		}
		UVector2I& operator-=(const UVector2I& other) {

			this->x -= other.x;
			this->y -= other.y;
			return *this;

		}
		UVector2I& operator-=(const uint32_t& other) {

			this->x -= other;
			this->y -= other;
			return *this;

		}
		UVector2I& operator*=(const UVector2I& other) {

			this->x *= other.x;
			this->y *= other.y;
			return *this;

		}
		UVector2I& operator*=(const uint32_t& other) {

			this->x *= other;
			this->y *= other;
			return *this;

		}
		UVector2I& operator/=(const UVector2I& other) {

			this->x /= other.x;
			this->y /= other.y;
			return *this;

		}
		UVector2I& operator/=(const uint32_t& other) {

			this->x /= other;
			this->y /= other;
			return *this;

		}

		UVector2I& operator++() {

			++this->x;
			++this->y;
			return *this;

		}
		UVector2I& operator--() {

			--this->x;
			--this->y;
			return *this;

		}

		bool operator==(const UVector2I& other) const { return x == other.x && y == other.y; }
		bool operator!=(const UVector2I& other) const { return !(*this == other); }

		operator Vector2() const;

	};

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

			if(r != other.r || g != other.g || b != other.b || a != other.a) return false;

			return true;
			
		}
		bool operator!=(const Color& other) const {

			return !(*this == other);

		}

	};
	
	inline std::ostream& operator<<(std::ostream& os, const Vector2& vec) {

		return os << "X: " << vec.x << ", Y: " << vec.y;

	}
	inline std::ostream& operator<<(std::ostream& os, const Vector3& vec) {

		return os << "X: " << vec.x << ", Y: " << vec.y << ", Z: " << vec.z;

	}
	inline std::ostream& operator<<(std::ostream& os, const Vector4& vec) {

		return os << "X: " << vec.x << ", Y: " << vec.y << ", Z: " << vec.z << ", W: " << vec.w;

	}
	inline std::ostream& operator<<(std::ostream& os, const UVector2I& vec) {

		return os << "X: " << vec.x << ", Y: " << vec.y;

	}
	inline std::ostream& operator<<(std::ostream& os, const Color& col) {

		return os << "R: " << col.r << ", G: " << col.g << ", B: " << col.b << ", A: " << col.a;

	}
	
	inline YAML::Emitter& operator<<(YAML::Emitter& out, const Vector2& vec) {

		out << YAML::Flow;
		out << YAML::BeginSeq << vec.x << vec.y << YAML::EndSeq;
		return out;

	}
	inline YAML::Emitter& operator<<(YAML::Emitter& out, const Vector3& vec) {

		out << YAML::Flow;
		out << YAML::BeginSeq << vec.x << vec.y << vec.z << YAML::EndSeq;
		return out;

	}
	inline YAML::Emitter& operator<<(YAML::Emitter& out, const Vector4& vec) {

		out << YAML::Flow;
		out << YAML::BeginSeq << vec.x << vec.y << vec.z << vec.w << YAML::EndSeq;
		return out;

	}
	inline YAML::Emitter& operator<<(YAML::Emitter& out, const Color& col) {

		out << YAML::Flow;
		out << YAML::BeginSeq << col.r << col.g << col.b << col.a << YAML::EndSeq;
		return out;

	}

}

namespace YAML {

	using namespace Copper;

	template<> struct convert<Vector2> {

		static Node encode(const Vector2& vec) {

			Node node;

			node.push_back(vec.x);
			node.push_back(vec.y);
			node.SetStyle(EmitterStyle::Flow);

			return node;

		}
		static bool decode(const Node& node, Vector2& vec) {

			if (!node.IsSequence() || node.size() != 2) return false;

			vec.x = node[0].as<float>();
			vec.y = node[1].as<float>();

			return true;

		}

	};
	template<> struct convert<Vector3> {

		static Node encode(const Vector3& vec) {

			Node node;

			node.push_back(vec.x);
			node.push_back(vec.y);
			node.push_back(vec.z);
			node.SetStyle(EmitterStyle::Flow);

			return node;

		}
		static bool decode(const Node& node, Vector3& vec) {

			if (!node.IsSequence() || node.size() != 3) return false;

			vec.x = node[0].as<float>();
			vec.y = node[1].as<float>();
			vec.z = node[2].as<float>();

			return true;

		}

	};
	template<> struct convert<Vector4> {

		static Node encode(const Vector4& vec) {

			Node node;

			node.push_back(vec.x);
			node.push_back(vec.y);
			node.push_back(vec.z);
			node.push_back(vec.w);
			node.SetStyle(EmitterStyle::Flow);

			return node;

		}
		static bool decode(const Node& node, Vector4& vec) {

			if (!node.IsSequence() || node.size() != 4) return false;

			vec.x = node[0].as<float>();
			vec.y = node[1].as<float>();
			vec.z = node[2].as<float>();
			vec.w = node[3].as<float>();

			return true;

		}

	};

	template<> struct convert<UVector2I> {

		static Node encode(const UVector2I& vec) {

			Node node;

			node.push_back(vec.x);
			node.push_back(vec.y);
			node.SetStyle(EmitterStyle::Flow);

			return node;

		}
		static bool decode(const Node& node, UVector2I& vec) {

			if (!node.IsSequence() || node.size() != 2) return false;

			vec.x = node[0].as<uint32_t>();
			vec.y = node[1].as<uint32_t>();

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