#pragma once

#include "Engine/Core/Core.h"

#include <GLM/vec2.hpp>
#include <GLM/vec3.hpp>
#include <GLM/vec4.hpp>

namespace Copper {

	struct Vector2 {

		Vector2() = default;
		Vector2(float all) : x(all), y(all) {}
		Vector2(float x, float y) : x(x), y(y) {}

		float x;
		float y;

		static Vector2 Zero() { return Vector2(0.0f); }
		static Vector2 One() { return Vector2(1.0f); }
		static Vector2 MinusOne() { return Vector2(-1.0f); }

		operator glm::vec2() const { return glm::vec2(x, y); }

		Vector2& operator+(const Vector2& other) const		{ return Vector2(x + other.x, y + other.y); }
		Vector2& operator+(const float other) const			{ return Vector2(x + other,   y + other); }
		Vector2& operator+(const glm::vec2& other) const	{ return Vector2(x + other.x, y + other.y); }
		Vector2& operator-(const Vector2& other) const		{ return Vector2(x - other.x, y - other.y); }
		Vector2& operator-(const float other) const			{ return Vector2(x - other,   y - other); }
		Vector2& operator-(const glm::vec2& other) const	{ return Vector2(x - other.x, y - other.y); }
		Vector2& operator*(const Vector2& other) const		{ return Vector2(x * other.x, y * other.y); }
		Vector2& operator*(const float other) const			{ return Vector2(x * other,   y * other); }
		Vector2& operator*(const glm::vec2& other) const	{ return Vector2(x * other.x, y * other.y); }
		Vector2& operator/(const Vector2& other) const		{ return Vector2(x / other.x, y / other.y); }
		Vector2& operator/(const float other) const			{ return Vector2(x / other,   y / other); }
		Vector2& operator/(const glm::vec2& other) const	{ return Vector2(x / other.x, y / other.y); }

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

		Vector2& operator-() const {

			return Vector2(-x, -y);

		}


	};
	struct Vector3 {

		Vector3() = default;
		Vector3(float all) : x(all), y(all), z(all) {}
		Vector3(float x, float y, float z) : x(x), y(y), z(z) {}

		float x;
		float y;
		float z;

		static Vector3 Zero() { return Vector3(0.0f); }
		static Vector3 One() { return Vector3(1.0f); }
		static Vector3 MinusOne() { return Vector3(-1.0f); }

		operator glm::vec3() const { return glm::vec3(x, y, z); }

		Vector3& operator+(const Vector3& other) const		{ return Vector3(x + other.x, y + other.y, z + other.z); }
		Vector3& operator+(const float other) const			{ return Vector3(x + other,   y + other,   z + other); }
		Vector3& operator+(const glm::vec3& other) const	{ return Vector3(x + other.x, y + other.y, z + other.z); }
		Vector3& operator-(const Vector3& other) const		{ return Vector3(x - other.x, y - other.y, z - other.z); }
		Vector3& operator-(const float other) const			{ return Vector3(x - other,   y - other,   z - other); }
		Vector3& operator-(const glm::vec3& other) const	{ return Vector3(x - other.x, y - other.y, z - other.z); }
		Vector3& operator*(const Vector3& other) const		{ return Vector3(x * other.x, y * other.y, z * other.z); }
		Vector3& operator*(const float other) const			{ return Vector3(x * other,   y * other,   z * other); }
		Vector3& operator*(const glm::vec3& other) const	{ return Vector3(x * other.x, y * other.y, z * other.z); }
		Vector3& operator/(const Vector3& other) const		{ return Vector3(x / other.x, y / other.y, z / other.z); }
		Vector3& operator/(const float other) const			{ return Vector3(x / other,   y / other,   z / other); }
		Vector3& operator/(const glm::vec3& other) const	{ return Vector3(x / other.x, y / other.y, z / other.z); }

		Vector3& operator=(const Vector3& other) {

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

		Vector3& operator-() const {

			return Vector3(-x, -y, -z);

		}


	};
	struct Vector4 {

		Vector4() = default;
		Vector4(float all) : x(all), y(all), z(all), w(all) {}
		Vector4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}

		float x;
		float y;
		float z;
		float w;

		static Vector4 Zero() { return Vector4(0.0f); }
		static Vector4 One() { return Vector4(1.0f); }
		static Vector4 MinusOne() { return Vector4(-1.0f); }

		operator glm::vec4() const { return glm::vec4(x, y, z, w); }

		Vector4& operator+(const Vector4& other) const		{ return Vector4(x + other.x, y + other.y, z + other.z, w + other.w); }
		Vector4& operator+(const float other) const			{ return Vector4(x + other,   y + other,   z + other,   w + other); }
		Vector4& operator+(const glm::vec4& other) const	{ return Vector4(x + other.x, y + other.y, z + other.z, w + other.w); }
		Vector4& operator-(const Vector4& other) const		{ return Vector4(x - other.x, y - other.y, z - other.z, w - other.w); }
		Vector4& operator-(const float other) const			{ return Vector4(x - other,   y - other,   z - other,   w - other); }
		Vector4& operator-(const glm::vec4& other) const	{ return Vector4(x - other.x, y - other.y, z - other.z, w - other.w); }
		Vector4& operator*(const Vector4& other) const		{ return Vector4(x * other.x, y * other.y, z * other.z, w * other.w); }
		Vector4& operator*(const float other) const			{ return Vector4(x * other,   y * other,   z * other,   w * other); }
		Vector4& operator*(const glm::vec4& other) const	{ return Vector4(x * other.x, y * other.y, z * other.z, w * other.w); }
		Vector4& operator/(const Vector4& other) const		{ return Vector4(x / other.x, y / other.y, z / other.z, w / other.w); }
		Vector4& operator/(const float other) const			{ return Vector4(x / other,   y / other,   z / other,   w / other); }
		Vector4& operator/(const glm::vec4& other) const	{ return Vector4(x / other.x, y / other.y, z / other.z, w / other.w); }

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

		Vector4& operator-() const {

			return Vector4(-x, -y, -z, -w);

		}


	};

	struct UVector2I {

		UVector2I() = default;
		UVector2I(uint32_t all) : x(all), y(all) {}
		UVector2I(uint32_t x, uint32_t y) : x(x), y(y) {}

		uint32_t x;
		uint32_t y;

		static UVector2I Zero() { return UVector2I(0); }
		static UVector2I One() { return UVector2I(1); }

		UVector2I& operator+(const UVector2I& other) const		{ return UVector2I(x + other.x, y + other.y); }
		UVector2I& operator+(const uint32_t other) const		{ return UVector2I(x + other,   y + other); }
		UVector2I& operator+(const glm::uvec2& other) const		{ return UVector2I(x + other.x, y + other.y); }
		UVector2I& operator-(const UVector2I& other) const		{ return UVector2I(x - other.x, y - other.y); }
		UVector2I& operator-(const uint32_t other) const		{ return UVector2I(x - other,   y - other); }
		UVector2I& operator-(const glm::uvec2& other) const		{ return UVector2I(x - other.x, y - other.y); }
		UVector2I& operator*(const UVector2I& other) const		{ return UVector2I(x * other.x, y * other.y); }
		UVector2I& operator*(const uint32_t other) const		{ return UVector2I(x * other,   y * other); }
		UVector2I& operator*(const glm::uvec2& other) const		{ return UVector2I(x * other.x, y * other.y); }
		UVector2I& operator/(const UVector2I& other) const		{ return UVector2I(x / other.x, y / other.y); }
		UVector2I& operator/(const uint32_t other) const		{ return UVector2I(x / other,   y / other); }
		UVector2I& operator/(const glm::uvec2& other) const		{ return UVector2I(x / other.x, y / other.y); }

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

	};

	struct Color {

		Color() = default;
		Color(float all, float alpha = 1.0f) : r(all), g(all), b(all), a(alpha) {}
		Color(float r, float g, float b, float alpha = 1.0f) : r(r), g(g), b(b), a(alpha) {}

		float r;
		float g;
		float b;
		float a;
		
		operator Vector3() const { return Vector3(r, g, b); }
		operator Vector4() const { return Vector4(r, g, b, a); }

		//operator bool() const { return *this != Color(-1.0f); }

		static Color Red() {   return Color(1.0f, 0.0f, 0.0f); }
		static Color Green() { return Color(0.0f, 1.0f, 0.0f); }
		static Color Blue() {  return Color(0.0f, 0.0f, 1.0f); }
		static Color White() { return Color(1.0f); }
		static Color Black() { return Color(0.0f); }
		static Color None() { return Color(-1.0f); }

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
	inline std::ostream& operator<<(std::ostream& os, const Color& col) {

		return os << "R: " << col.r << ", G: " << col.g << ", B: " << col.b << ", A: " << col.a;

	}
	
}