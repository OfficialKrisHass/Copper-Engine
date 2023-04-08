#pragma once

#include "Engine/Core/Core.h"

#include <GLM/vec2.hpp>
#include <GLM/vec3.hpp>
#include <GLM/vec4.hpp>

#include <yaml-cpp/yaml.h>

namespace Copper {

	struct Vector2I;
	struct UVector2I;

	struct Vector3I;
	struct UVector3I;

	struct Vector4I;
	struct UVector4I;

	struct Vector2 {

		Vector2() = default;
		Vector2(float all) : x(all), y(all) {}
		Vector2(float x, float y) : x(x), y(y) {}
		Vector2(const glm::vec2& v) : x(v.x), y(v.y) {}

		float x = 0.0f;
		float y = 0.0f;

		static Vector2 zero;
		static Vector2 one;
		static Vector2 minusOne;

		inline float Dot(const Vector2& other) const { return x * other.x + y * other.y; }
		inline float LengthSq() const { return x * x + y * y; }
		inline float Length() const { return sqrt(x * x + y * y); }

		inline Vector2& Normalize() { return *this /= x * x + y * y; }

		Vector2 operator+(const Vector2& other) const		{ return Vector2(x + other.x, y + other.y); }
		Vector2 operator+(const glm::vec2& other) const		{ return Vector2(x + other.x, y + other.y); }
		Vector2 operator+(const float other) const			{ return Vector2(x + other,   y + other); }
		Vector2 operator-(const Vector2& other) const		{ return Vector2(x - other.x, y - other.y); }
		Vector2 operator-(const glm::vec2& other) const		{ return Vector2(x - other.x, y - other.y); }
		Vector2 operator-(const float other) const			{ return Vector2(x - other,   y - other); }
		Vector2 operator*(const Vector2& other) const		{ return Vector2(x * other.x, y * other.y); }
		Vector2 operator*(const glm::vec2& other) const		{ return Vector2(x * other.x, y * other.y); }
		Vector2 operator*(const float other) const			{ return Vector2(x * other,   y * other); }
		Vector2 operator/(const Vector2& other) const		{ return Vector2(x / other.x, y / other.y); }
		Vector2 operator/(const glm::vec2& other) const		{ return Vector2(x / other.x, y / other.y); }
		Vector2 operator/(const float other) const			{ return Vector2(x / other,   y / other); }

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
		Vector2& operator+=(const float other) {

			this->x += other;
			this->y += other;
			return *this;

		}
		Vector2& operator-=(const Vector2& other) {

			this->x -= other.x;
			this->y -= other.y;
			return *this;

		}
		Vector2& operator-=(const float other) {

			this->x -= other;
			this->y -= other;
			return *this;

		}
		Vector2& operator*=(const Vector2& other) {

			this->x *= other.x;
			this->y *= other.y;
			return *this;

		}
		Vector2& operator*=(const float other) {

			this->x *= other;
			this->y *= other;
			return *this;

		}
		Vector2& operator/=(const Vector2& other) {

			this->x /= other.x;
			this->y /= other.y;
			return *this;

		}
		Vector2& operator/=(const float other) {

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

		Vector2 operator-() const { return Vector2(-x, -y); }

		bool operator==(const Vector2& other) const { return x == other.x && y == other.y; }
		bool operator!=(const Vector2& other) const { return !(*this == other); }

		operator UVector2I() const;
		operator Vector2I() const;

		operator glm::vec2() const { return glm::vec2(x, y); }

	};
	struct Vector3 {

		Vector3() = default;
		Vector3(float all) : x(all), y(all), z(all) {}
		Vector3(float x, float y, float z = 0.0f) : x(x), y(y), z(z) {}
		Vector3(const Vector2& v) : x(v.x), y(v.y), z(0.0f) {}
		Vector3(const glm::vec3& vec) : x(vec.x), y(vec.y), z(vec.z) {}

		float x = 0.0f;
		float y = 0.0f;
		float z = 0.0f;

		static Vector3 zero;
		static Vector3 one;
		static Vector3 minusOne;

		inline float Dot(const Vector3& other) const { return x * other.x + y * other.y + z * other.z; }
		inline float LengthSq() const { return x * x + y * y + z * z; }
		inline float Length() const { return sqrt(x * x + y * y + z * z); }

		inline Vector3& Normalize() { return *this /= x * x + y * y + z * z; }

		operator glm::vec3() const { return glm::vec3(x, y, z); }

		Vector3 operator+(const Vector3& other) const		{ return Vector3(x + other.x, y + other.y, z + other.z); }
		Vector3 operator+(const glm::vec3& other) const		{ return Vector3(x + other.x, y + other.y, z + other.z); }
		Vector3 operator+(const float other) const			{ return Vector3(x + other,   y + other,   z + other); }
		Vector3 operator-(const Vector3& other) const		{ return Vector3(x - other.x, y - other.y, z - other.z); }
		Vector3 operator-(const glm::vec3& other) const		{ return Vector3(x - other.x, y - other.y, z - other.z); }
		Vector3 operator-(const float other) const			{ return Vector3(x - other,   y - other,   z - other); }
		Vector3 operator*(const Vector3& other) const		{ return Vector3(x * other.x, y * other.y, z * other.z); }
		Vector3 operator*(const glm::vec3& other) const		{ return Vector3(x * other.x, y * other.y, z * other.z); }
		Vector3 operator*(const float other) const			{ return Vector3(x * other,   y * other,   z * other); }
		Vector3 operator/(const Vector3& other) const		{ return Vector3(x / other.x, y / other.y, z / other.z); }
		Vector3 operator/(const glm::vec3& other) const		{ return Vector3(x / other.x, y / other.y, z / other.z); }
		Vector3 operator/(const float other) const			{ return Vector3(x / other,   y / other,   z / other); }

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
		Vector3& operator+=(const float other) {

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
		Vector3& operator-=(const glm::vec3& other) {

			this->x -= other.x;
			this->y -= other.y;
			this->z -= other.z;
			return *this;

		}
		Vector3& operator-=(const float other) {

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
		Vector3& operator*=(const glm::vec3& other) {

			this->x *= other.x;
			this->y *= other.y;
			this->z *= other.z;
			return *this;

		}
		Vector3& operator*=(const float other) {

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
		Vector3& operator/=(const glm::vec3& other) {

			this->x /= other.x;
			this->y /= other.y;
			this->z /= other.z;
			return *this;

		}
		Vector3& operator/=(const float other) {

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

		operator Vector3I() const;
		operator UVector3I() const;


	};
	struct Vector4 {

		Vector4() = default;
		Vector4(float all) : x(all), y(all), z(all), w(all) {}
		Vector4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}
		Vector4(const Vector3& v) : x(v.x), y(v.y), z(v.z), w(0.0f) {}
		Vector4(const glm::vec3& v) : x(v.x), y(v.y), z(v.z), w(0.0f) {}

		float x = 0.0f;
		float y = 0.0f;
		float z = 0.0f;
		float w = 0.0f;

		static Vector4 zero;
		static Vector4 one;
		static Vector4 minusOne;

		inline float Dot(const Vector4& other) const { return x * other.x + y * other.y + z * other.z + w * other.w; }
		inline float LengthSq() const { return x * x + y * y + z * z + w * w; }
		inline float Length() const { return sqrt(x * x + y * y + z * z + w * w); }

		inline Vector4& Normalize() { return *this /= x * x + y * y + z * z + w * w; }

		operator glm::vec4() const { return glm::vec4(x, y, z, w); }

		Vector4 operator+(const Vector4& other) const		{ return Vector4(x + other.x, y + other.y, z + other.z, w + other.w); }
		Vector4 operator+(const glm::vec4& other) const		{ return Vector4(x + other.x, y + other.y, z + other.z, w + other.w); }
		Vector4 operator+(const float other) const			{ return Vector4(x + other,   y + other,   z + other,   w + other); }
		Vector4 operator-(const Vector4& other) const		{ return Vector4(x - other.x, y - other.y, z - other.z, w - other.w); }
		Vector4 operator-(const glm::vec4& other) const		{ return Vector4(x - other.x, y - other.y, z - other.z, w - other.w); }
		Vector4 operator-(const float other) const			{ return Vector4(x - other,   y - other,   z - other,   w - other); }
		Vector4 operator*(const Vector4& other) const		{ return Vector4(x * other.x, y * other.y, z * other.z, w * other.w); }
		Vector4 operator*(const glm::vec4& other) const		{ return Vector4(x * other.x, y * other.y, z * other.z, w * other.w); }
		Vector4 operator*(const float other) const			{ return Vector4(x * other,   y * other,   z * other,   w * other); }
		Vector4 operator/(const Vector4& other) const		{ return Vector4(x / other.x, y / other.y, z / other.z, w / other.w); }
		Vector4 operator/(const glm::vec4& other) const		{ return Vector4(x / other.x, y / other.y, z / other.z, w / other.w); }
		Vector4 operator/(const float other) const			{ return Vector4(x / other,   y / other,   z / other,   w / other); }

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

		operator Vector4I() const;
		operator UVector4I() const;

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

		operator Vector2() const { return Vector2((float) x, (float) y); }
		operator UVector2I() const;

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

		operator Vector2() const { return Vector2((float) x, (float) y); }
		operator Vector2I() const { return Vector2I((int32_t) x, (int32_t) y); }

	};

	struct Vector3I {

		Vector3I() = default;
		Vector3I(int32_t all) : x(all), y(all), z(all) {}
		Vector3I(int32_t x, int32_t y, int32_t z = 0.0f) : x(x), y(y), z(z) {}
		Vector3I(const Vector2& v) : x(v.x), y(v.y), z(0.0f) {}
		Vector3I(const glm::vec3& vec) : x(vec.x), y(vec.y), z(vec.z) {}

		int32_t x = 0.0f;
		int32_t y = 0.0f;
		int32_t z = 0.0f;

		static Vector3I zero;
		static Vector3I one;
		static Vector3I minusOne;

		int32_t Length() const {

			return sqrt(x * x + y * y + z * z);

		}

		operator glm::vec3() const { return glm::vec3(x, y, z); }

		Vector3I operator+(const Vector3I& other) const { return Vector3I(x + other.x, y + other.y, z + other.z); }
		Vector3I operator+(const glm::vec3& other) const { return Vector3I(x + other.x, y + other.y, z + other.z); }
		Vector3I operator+(const int32_t other) const { return Vector3I(x + other, y + other, z + other); }
		Vector3I operator-(const Vector3I& other) const { return Vector3I(x - other.x, y - other.y, z - other.z); }
		Vector3I operator-(const glm::vec3& other) const { return Vector3I(x - other.x, y - other.y, z - other.z); }
		Vector3I operator-(const int32_t other) const { return Vector3I(x - other, y - other, z - other); }
		Vector3I operator*(const Vector3I& other) const { return Vector3I(x * other.x, y * other.y, z * other.z); }
		Vector3I operator*(const glm::vec3& other) const { return Vector3I(x * other.x, y * other.y, z * other.z); }
		Vector3I operator*(const int32_t other) const { return Vector3I(x * other, y * other, z * other); }
		Vector3I operator/(const Vector3I& other) const { return Vector3I(x / other.x, y / other.y, z / other.z); }
		Vector3I operator/(const glm::vec3& other) const { return Vector3I(x / other.x, y / other.y, z / other.z); }
		Vector3I operator/(const int32_t other) const { return Vector3I(x / other, y / other, z / other); }

		Vector3I& operator=(const Vector3I& other) {

			this->x = other.x;
			this->y = other.y;
			this->z = other.z;
			return *this;

		}
		Vector3I& operator=(const glm::vec3& other) {

			this->x = other.x;
			this->y = other.y;
			this->z = other.z;
			return *this;

		}

		Vector3I& operator+=(const Vector3I& other) {

			this->x += other.x;
			this->y += other.y;
			this->z += other.z;
			return *this;

		}
		Vector3I& operator+=(const glm::vec3& other) {

			this->x += other.x;
			this->y += other.y;
			this->z += other.z;
			return *this;

		}
		Vector3I& operator+=(const int32_t other) {

			this->x += other;
			this->y += other;
			this->z += other;
			return *this;

		}
		Vector3I& operator-=(const Vector3I& other) {

			this->x -= other.x;
			this->y -= other.y;
			this->z -= other.z;
			return *this;

		}
		Vector3I& operator-=(const glm::vec3& other) {

			this->x -= other.x;
			this->y -= other.y;
			this->z -= other.z;
			return *this;

		}
		Vector3I& operator-=(const int32_t other) {

			this->x -= other;
			this->y -= other;
			this->z -= other;
			return *this;

		}
		Vector3I& operator*=(const Vector3I& other) {

			this->x *= other.x;
			this->y *= other.y;
			this->z *= other.z;
			return *this;

		}
		Vector3I& operator*=(const glm::vec3& other) {

			this->x *= other.x;
			this->y *= other.y;
			this->z *= other.z;
			return *this;

		}
		Vector3I& operator*=(const int32_t other) {

			this->x *= other;
			this->y *= other;
			this->z *= other;
			return *this;

		}
		Vector3I& operator/=(const Vector3I& other) {

			this->x /= other.x;
			this->y /= other.y;
			this->z /= other.z;
			return *this;

		}
		Vector3I& operator/=(const glm::vec3& other) {

			this->x /= other.x;
			this->y /= other.y;
			this->z /= other.z;
			return *this;

		}
		Vector3I& operator/=(const int32_t other) {

			this->x /= other;
			this->y /= other;
			this->z /= other;
			return *this;

		}

		Vector3I& operator++() {

			++this->x;
			++this->y;
			++this->z;
			return *this;

		}
		Vector3I& operator--() {

			--this->x;
			--this->y;
			--this->z;
			return *this;

		}

		bool operator==(const Vector3I& other) const { return x == other.x && y == other.y && z == other.z; }
		bool operator!=(const Vector3I& other) const { return !(*this == other); }

		Vector3I operator-() const { return Vector3I(-x, -y, -z); }

		operator Vector3() const { return Vector3((float) x, (float) y, (float) z); }
		operator UVector3I() const;


	};
	struct UVector3I {

		UVector3I() = default;
		UVector3I(uint32_t all) : x(all), y(all), z(all) {}
		UVector3I(uint32_t x, uint32_t y, uint32_t z = 0.0f) : x(x), y(y), z(z) {}
		UVector3I(const Vector2& v) : x(v.x), y(v.y), z(0.0f) {}
		UVector3I(const glm::vec3& vec) : x(vec.x), y(vec.y), z(vec.z) {}

		uint32_t x = 0.0f;
		uint32_t y = 0.0f;
		uint32_t z = 0.0f;

		static UVector3I zero;
		static UVector3I one;

		operator glm::vec3() const { return glm::vec3(x, y, z); }

		UVector3I operator+(const UVector3I& other) const { return UVector3I(x + other.x, y + other.y, z + other.z); }
		UVector3I operator+(const glm::vec3& other) const { return UVector3I(x + other.x, y + other.y, z + other.z); }
		UVector3I operator+(const uint32_t other) const { return UVector3I(x + other, y + other, z + other); }
		UVector3I operator-(const UVector3I& other) const { return UVector3I(x - other.x, y - other.y, z - other.z); }
		UVector3I operator-(const glm::vec3& other) const { return UVector3I(x - other.x, y - other.y, z - other.z); }
		UVector3I operator-(const uint32_t other) const { return UVector3I(x - other, y - other, z - other); }
		UVector3I operator*(const UVector3I& other) const { return UVector3I(x * other.x, y * other.y, z * other.z); }
		UVector3I operator*(const glm::vec3& other) const { return UVector3I(x * other.x, y * other.y, z * other.z); }
		UVector3I operator*(const uint32_t other) const { return UVector3I(x * other, y * other, z * other); }
		UVector3I operator/(const UVector3I& other) const { return UVector3I(x / other.x, y / other.y, z / other.z); }
		UVector3I operator/(const glm::vec3& other) const { return UVector3I(x / other.x, y / other.y, z / other.z); }
		UVector3I operator/(const uint32_t other) const { return UVector3I(x / other, y / other, z / other); }

		UVector3I& operator=(const UVector3I& other) {

			this->x = other.x;
			this->y = other.y;
			this->z = other.z;
			return *this;

		}
		UVector3I& operator=(const glm::vec3& other) {

			this->x = other.x;
			this->y = other.y;
			this->z = other.z;
			return *this;

		}

		UVector3I& operator+=(const UVector3I& other) {

			this->x += other.x;
			this->y += other.y;
			this->z += other.z;
			return *this;

		}
		UVector3I& operator+=(const glm::vec3& other) {

			this->x += other.x;
			this->y += other.y;
			this->z += other.z;
			return *this;

		}
		UVector3I& operator+=(const uint32_t other) {

			this->x += other;
			this->y += other;
			this->z += other;
			return *this;

		}
		UVector3I& operator-=(const UVector3I& other) {

			this->x -= other.x;
			this->y -= other.y;
			this->z -= other.z;
			return *this;

		}
		UVector3I& operator-=(const glm::vec3& other) {

			this->x -= other.x;
			this->y -= other.y;
			this->z -= other.z;
			return *this;

		}
		UVector3I& operator-=(const uint32_t other) {

			this->x -= other;
			this->y -= other;
			this->z -= other;
			return *this;

		}
		UVector3I& operator*=(const UVector3I& other) {

			this->x *= other.x;
			this->y *= other.y;
			this->z *= other.z;
			return *this;

		}
		UVector3I& operator*=(const glm::vec3& other) {

			this->x *= other.x;
			this->y *= other.y;
			this->z *= other.z;
			return *this;

		}
		UVector3I& operator*=(const uint32_t other) {

			this->x *= other;
			this->y *= other;
			this->z *= other;
			return *this;

		}
		UVector3I& operator/=(const UVector3I& other) {

			this->x /= other.x;
			this->y /= other.y;
			this->z /= other.z;
			return *this;

		}
		UVector3I& operator/=(const glm::vec3& other) {

			this->x /= other.x;
			this->y /= other.y;
			this->z /= other.z;
			return *this;

		}
		UVector3I& operator/=(const uint32_t other) {

			this->x /= other;
			this->y /= other;
			this->z /= other;
			return *this;

		}

		UVector3I& operator++() {

			++this->x;
			++this->y;
			++this->z;
			return *this;

		}
		UVector3I& operator--() {

			--this->x;
			--this->y;
			--this->z;
			return *this;

		}

		bool operator==(const UVector3I& other) const { return x == other.x && y == other.y && z == other.z; }
		bool operator!=(const UVector3I& other) const { return !(*this == other); }

		operator Vector3() const { return Vector3((float) x, (float) y, (float) z); }
		operator Vector3I() const { return Vector3I((int32_t) x, (int32_t) y, (int32_t) z); }

	};

	struct Vector4I {

		Vector4I() = default;
		Vector4I(int32_t all) : x(all), y(all), z(all), w(all) {}
		Vector4I(int32_t x, int32_t y, int32_t z, int32_t w) : x(x), y(y), z(z), w(w) {}
		Vector4I(const Vector3& v) : x(v.x), y(v.y), z(v.z), w(0.0f) {}
		Vector4I(const glm::vec3& v) : x(v.x), y(v.y), z(v.z), w(0.0f) {}

		int32_t x = 0.0f;
		int32_t y = 0.0f;
		int32_t z = 0.0f;
		int32_t w = 0.0f;

		static Vector4I zero;
		static Vector4I one;
		static Vector4I minusOne;

		operator glm::vec4() const { return glm::vec4(x, y, z, w); }

		Vector4I operator+(const Vector4I& other) const { return Vector4I(x + other.x, y + other.y, z + other.z, w + other.w); }
		Vector4I operator+(const glm::vec4& other) const { return Vector4I(x + other.x, y + other.y, z + other.z, w + other.w); }
		Vector4I operator+(const int32_t other) const { return Vector4I(x + other, y + other, z + other, w + other); }
		Vector4I operator-(const Vector4I& other) const { return Vector4I(x - other.x, y - other.y, z - other.z, w - other.w); }
		Vector4I operator-(const glm::vec4& other) const { return Vector4I(x - other.x, y - other.y, z - other.z, w - other.w); }
		Vector4I operator-(const int32_t other) const { return Vector4I(x - other, y - other, z - other, w - other); }
		Vector4I operator*(const Vector4I& other) const { return Vector4I(x * other.x, y * other.y, z * other.z, w * other.w); }
		Vector4I operator*(const glm::vec4& other) const { return Vector4I(x * other.x, y * other.y, z * other.z, w * other.w); }
		Vector4I operator*(const int32_t other) const { return Vector4I(x * other, y * other, z * other, w * other); }
		Vector4I operator/(const Vector4I& other) const { return Vector4I(x / other.x, y / other.y, z / other.z, w / other.w); }
		Vector4I operator/(const glm::vec4& other) const { return Vector4I(x / other.x, y / other.y, z / other.z, w / other.w); }
		Vector4I operator/(const int32_t other) const { return Vector4I(x / other, y / other, z / other, w / other); }

		Vector4I& operator=(const Vector4I& other) {

			this->x = other.x;
			this->y = other.y;
			this->z = other.z;
			this->w = other.w;
			return *this;

		}
		Vector4I& operator+=(const Vector4I& other) {

			this->x += other.x;
			this->y += other.y;
			this->z += other.z;
			this->w += other.w;
			return *this;

		}
		Vector4I& operator+=(const int32_t& other) {

			this->x += other;
			this->y += other;
			this->z += other;
			this->w += other;
			return *this;

		}
		Vector4I& operator-=(const Vector4I& other) {

			this->x -= other.x;
			this->y -= other.y;
			this->z -= other.z;
			this->w -= other.w;
			return *this;

		}
		Vector4I& operator-=(const int32_t& other) {

			this->x -= other;
			this->y -= other;
			this->z -= other;
			this->w -= other;
			return *this;

		}
		Vector4I& operator*=(const Vector4I& other) {

			this->x *= other.x;
			this->y *= other.y;
			this->z *= other.z;
			this->w *= other.w;
			return *this;

		}
		Vector4I& operator*=(const int32_t& other) {

			this->x *= other;
			this->y *= other;
			this->z *= other;
			this->w *= other;
			return *this;

		}
		Vector4I& operator/=(const Vector4I& other) {

			this->x /= other.x;
			this->y /= other.y;
			this->z /= other.z;
			this->w /= other.w;
			return *this;

		}
		Vector4I& operator/=(const int32_t& other) {

			this->x /= other;
			this->y /= other;
			this->z /= other;
			this->w /= other;
			return *this;

		}

		Vector4I& operator++() {

			++this->x;
			++this->y;
			++this->z;
			++this->w;
			return *this;

		}
		Vector4I& operator--() {

			--this->x;
			--this->y;
			--this->z;
			--this->w;
			return *this;

		}

		bool operator==(const Vector4I& other) const { return x == other.x && y == other.y && z == other.z && w == other.w; }
		bool operator!=(const Vector4I& other) const { return !(*this == other); }

		Vector4I operator-() const { return Vector4I(-x, -y, -z, -w); }

		operator Vector4() const { return Vector4((float) x, (float) y, (float) z, (float) w); }
		operator UVector4I() const;

	};
	struct UVector4I {

		UVector4I() = default;
		UVector4I(uint32_t all) : x(all), y(all), z(all), w(all) {}
		UVector4I(uint32_t x, uint32_t y, uint32_t z, uint32_t w) : x(x), y(y), z(z), w(w) {}
		UVector4I(const Vector3& v) : x(v.x), y(v.y), z(v.z), w(0.0f) {}
		UVector4I(const glm::vec3& v) : x(v.x), y(v.y), z(v.z), w(0.0f) {}

		uint32_t x = 0.0f;
		uint32_t y = 0.0f;
		uint32_t z = 0.0f;
		uint32_t w = 0.0f;

		static UVector4I zero;
		static UVector4I one;

		operator glm::vec4() const { return glm::vec4(x, y, z, w); }

		UVector4I operator+(const UVector4I& other) const { return UVector4I(x + other.x, y + other.y, z + other.z, w + other.w); }
		UVector4I operator+(const glm::vec4& other) const { return UVector4I(x + other.x, y + other.y, z + other.z, w + other.w); }
		UVector4I operator+(const uint32_t other) const { return UVector4I(x + other, y + other, z + other, w + other); }
		UVector4I operator-(const UVector4I& other) const { return UVector4I(x - other.x, y - other.y, z - other.z, w - other.w); }
		UVector4I operator-(const glm::vec4& other) const { return UVector4I(x - other.x, y - other.y, z - other.z, w - other.w); }
		UVector4I operator-(const uint32_t other) const { return UVector4I(x - other, y - other, z - other, w - other); }
		UVector4I operator*(const UVector4I& other) const { return UVector4I(x * other.x, y * other.y, z * other.z, w * other.w); }
		UVector4I operator*(const glm::vec4& other) const { return UVector4I(x * other.x, y * other.y, z * other.z, w * other.w); }
		UVector4I operator*(const uint32_t other) const { return UVector4I(x * other, y * other, z * other, w * other); }
		UVector4I operator/(const UVector4I& other) const { return UVector4I(x / other.x, y / other.y, z / other.z, w / other.w); }
		UVector4I operator/(const glm::vec4& other) const { return UVector4I(x / other.x, y / other.y, z / other.z, w / other.w); }
		UVector4I operator/(const uint32_t other) const { return UVector4I(x / other, y / other, z / other, w / other); }

		UVector4I& operator=(const UVector4I& other) {

			this->x = other.x;
			this->y = other.y;
			this->z = other.z;
			this->w = other.w;
			return *this;

		}
		UVector4I& operator+=(const UVector4I& other) {

			this->x += other.x;
			this->y += other.y;
			this->z += other.z;
			this->w += other.w;
			return *this;

		}
		UVector4I& operator+=(const uint32_t& other) {

			this->x += other;
			this->y += other;
			this->z += other;
			this->w += other;
			return *this;

		}
		UVector4I& operator-=(const UVector4I& other) {

			this->x -= other.x;
			this->y -= other.y;
			this->z -= other.z;
			this->w -= other.w;
			return *this;

		}
		UVector4I& operator-=(const uint32_t& other) {

			this->x -= other;
			this->y -= other;
			this->z -= other;
			this->w -= other;
			return *this;

		}
		UVector4I& operator*=(const UVector4I& other) {

			this->x *= other.x;
			this->y *= other.y;
			this->z *= other.z;
			this->w *= other.w;
			return *this;

		}
		UVector4I& operator*=(const uint32_t& other) {

			this->x *= other;
			this->y *= other;
			this->z *= other;
			this->w *= other;
			return *this;

		}
		UVector4I& operator/=(const UVector4I& other) {

			this->x /= other.x;
			this->y /= other.y;
			this->z /= other.z;
			this->w /= other.w;
			return *this;

		}
		UVector4I& operator/=(const uint32_t& other) {

			this->x /= other;
			this->y /= other;
			this->z /= other;
			this->w /= other;
			return *this;

		}

		UVector4I& operator++() {

			++this->x;
			++this->y;
			++this->z;
			++this->w;
			return *this;

		}
		UVector4I& operator--() {

			--this->x;
			--this->y;
			--this->z;
			--this->w;
			return *this;

		}

		bool operator==(const UVector4I& other) const { return x == other.x && y == other.y && z == other.z && w == other.w; }
		bool operator!=(const UVector4I& other) const { return !(*this == other); }

		operator Vector4() const { return Vector4((float) x, (float) y, (float) z, (float) w); }
		operator Vector4I() const { return Vector4I((int32_t) x, (int32_t) y, (int32_t) z, (int32_t) w); }

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