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

		inline static Vector2 Zero() { return Vector2(0.0f); }
		inline static Vector2 One() { return Vector2(1.0f); }
		inline static Vector2 MinusOne() { return Vector2(-1.0f); }

		inline operator glm::vec2() const { return glm::vec2(x, y); }

		inline Vector2& operator=(const Vector2& other) {

			this->x = other.x;
			this->y = other.y;
			return *this;

		}
		inline Vector2& operator+=(const Vector2& other) {

			this->x += other.x;
			this->y += other.y;
			return *this;

		}
		inline Vector2& operator+=(const float& other) {

			this->x += other;
			this->y += other;
			return *this;

		}
		inline Vector2& operator-=(const Vector2& other) {

			this->x -= other.x;
			this->y -= other.y;
			return *this;

		}
		inline Vector2& operator-=(const float& other) {

			this->x -= other;
			this->y -= other;
			return *this;

		}
		inline Vector2& operator*=(const Vector2& other) {

			this->x *= other.x;
			this->y *= other.y;
			return *this;

		}
		inline Vector2& operator*=(const float& other) {

			this->x *= other;
			this->y *= other;
			return *this;

		}
		inline Vector2& operator/=(const Vector2& other) {

			this->x /= other.x;
			this->y /= other.y;
			return *this;

		}
		inline Vector2& operator/=(const float& other) {

			this->x /= other;
			this->y /= other;
			return *this;

		}

		inline Vector2& operator++() {

			++this->x;
			++this->y;
			return *this;

		}
		inline Vector2& operator--() {

			--this->x;
			--this->y;
			return *this;

		}

		inline Vector2& operator-(const Vector2& other) {

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

		inline static Vector3 Zero() { return Vector3(0.0f); }
		inline static Vector3 One() { return Vector3(1.0f); }
		inline static Vector3 MinusOne() { return Vector3(-1.0f); }

		inline operator glm::vec3() const { return glm::vec3(x, y, z); }

		inline Vector3& operator=(const Vector3& other) {

			this->x = other.x;
			this->y = other.y;
			this->z = other.z;
			return *this;

		}
		inline Vector3& operator+=(const Vector3& other) {

			this->x += other.x;
			this->y += other.y;
			this->z += other.z;
			return *this;

		}
		inline Vector3& operator+=(const float& other) {

			this->x += other;
			this->y += other;
			this->z += other;
			return *this;

		}
		inline Vector3& operator-=(const Vector3& other) {

			this->x -= other.x;
			this->y -= other.y;
			this->z -= other.z;
			return *this;

		}
		inline Vector3& operator-=(const float& other) {

			this->x -= other;
			this->y -= other;
			this->z -= other;
			return *this;

		}
		inline Vector3& operator*=(const Vector3& other) {

			this->x *= other.x;
			this->y *= other.y;
			this->z *= other.z;
			return *this;

		}
		inline Vector3& operator*=(const float& other) {

			this->x *= other;
			this->y *= other;
			this->z *= other;
			return *this;

		}
		inline Vector3& operator/=(const Vector3& other) {

			this->x /= other.x;
			this->y /= other.y;
			this->z /= other.z;
			return *this;

		}
		inline Vector3& operator/=(const float& other) {

			this->x /= other;
			this->y /= other;
			this->z /= other;
			return *this;

		}

		inline Vector3& operator++() {

			++this->x;
			++this->y;
			++this->z;
			return *this;

		}
		inline Vector3& operator--() {

			--this->x;
			--this->y;
			--this->z;
			return *this;

		}

		inline Vector3& operator-(const Vector3& other) {

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

		inline static Vector4 Zero() { return Vector4(0.0f); }
		inline static Vector4 One() { return Vector4(1.0f); }
		inline static Vector4 MinusOne() { return Vector4(-1.0f); }

		inline operator glm::vec4() const { return glm::vec4(x, y, z, w); }

		inline Vector4& operator=(const Vector4& other) {

			this->x = other.x;
			this->y = other.y;
			this->z = other.z;
			this->w = other.w;
			return *this;

		}
		inline Vector4& operator+=(const Vector4& other) {

			this->x += other.x;
			this->y += other.y;
			this->z += other.z;
			this->w += other.w;
			return *this;

		}
		inline Vector4& operator+=(const float& other) {

			this->x += other;
			this->y += other;
			this->z += other;
			this->w += other;
			return *this;

		}
		inline Vector4& operator-=(const Vector4& other) {

			this->x -= other.x;
			this->y -= other.y;
			this->z -= other.z;
			this->w -= other.w;
			return *this;

		}
		inline Vector4& operator-=(const float& other) {

			this->x -= other;
			this->y -= other;
			this->z -= other;
			this->w -= other;
			return *this;

		}
		inline Vector4& operator*=(const Vector4& other) {

			this->x *= other.x;
			this->y *= other.y;
			this->z *= other.z;
			this->w *= other.w;
			return *this;

		}
		inline Vector4& operator*=(const float& other) {

			this->x *= other;
			this->y *= other;
			this->z *= other;
			this->w *= other;
			return *this;

		}
		inline Vector4& operator/=(const Vector4& other) {

			this->x /= other.x;
			this->y /= other.y;
			this->z /= other.z;
			this->w /= other.w;
			return *this;

		}
		inline Vector4& operator/=(const float& other) {

			this->x /= other;
			this->y /= other;
			this->z /= other;
			this->w /= other;
			return *this;

		}

		inline Vector4& operator++() {

			++this->x;
			++this->y;
			++this->z;
			++this->w;
			return *this;

		}
		inline Vector4& operator--() {

			--this->x;
			--this->y;
			--this->z;
			--this->w;
			return *this;

		}

		inline Vector4& operator-(const Vector4& other) {

			return Vector4(-x, -y, -z, -w);

		}


	};

	struct UVector2I {

		UVector2I() = default;
		UVector2I(uint32_t all) : x(all), y(all) {}
		UVector2I(uint32_t x, uint32_t y) : x(x), y(y) {}

		uint32_t x;
		uint32_t y;

		inline static UVector2I Zero() { return UVector2I(0.0f); }
		inline static UVector2I One() { return UVector2I(1.0f); }
		inline static UVector2I MinusOne() { return UVector2I(-1.0f); }

		inline UVector2I& operator=(const UVector2I& other) {

			this->x = other.x;
			this->y = other.y;
			return *this;

		}
		inline UVector2I& operator+=(const UVector2I& other) {

			this->x += other.x;
			this->y += other.y;
			return *this;

		}
		inline UVector2I& operator+=(const uint32_t& other) {

			this->x += other;
			this->y += other;
			return *this;

		}
		inline UVector2I& operator-=(const UVector2I& other) {

			this->x -= other.x;
			this->y -= other.y;
			return *this;

		}
		inline UVector2I& operator-=(const uint32_t& other) {

			this->x -= other;
			this->y -= other;
			return *this;

		}
		inline UVector2I& operator*=(const UVector2I& other) {

			this->x *= other.x;
			this->y *= other.y;
			return *this;

		}
		inline UVector2I& operator*=(const uint32_t& other) {

			this->x *= other;
			this->y *= other;
			return *this;

		}
		inline UVector2I& operator/=(const UVector2I& other) {

			this->x /= other.x;
			this->y /= other.y;
			return *this;

		}
		inline UVector2I& operator/=(const uint32_t& other) {

			this->x /= other;
			this->y /= other;
			return *this;

		}

		inline UVector2I& operator++() {

			++this->x;
			++this->y;
			return *this;

		}
		inline UVector2I& operator--() {

			--this->x;
			--this->y;
			return *this;

		}

		inline UVector2I& operator-(const UVector2I& other) {

			return UVector2I(-x, -y);

		}

	};

	struct Color {

		Color() = default;
		Color(float all, float alpha = 1.0f) : r(all), g(all), b(all), a(alpha) {}
		Color(float r, float g, float b, float alpha = 1.0f) : r(r), g(g), b(b), a(alpha) {}

		float r;
		float g;
		float b;
		float a;

		inline static Color Red() {   return Color(1.0f, 0.0f, 0.0f); }
		inline static Color Green() { return Color(0.0f, 1.0f, 0.0f); }
		inline static Color Blue() {  return Color(0.0f, 0.0f, 1.0f); }
		inline static Color White() { return Color(1.0f); }
		inline static Color Black() { return Color(0.0f); }

	};

	inline Vector2& operator+(const Vector2& other, float scalar) {

		return Vector2(other.x + scalar, other.y + scalar);

	}
	inline Vector2& operator+(const Vector2& other, const Vector2& scalar) {

		return Vector2(other.x + scalar.x, other.y + scalar.y);

	}
	inline Vector2& operator+(float scalar, const Vector2& other) {

		return Vector2(scalar + other.x, scalar + other.y);

	}
	inline Vector2& operator-(const Vector2& other, float scalar) {

		return Vector2(other.x - scalar, other.y - scalar);

	}
	inline Vector2& operator-(const Vector2& other, const Vector2& scalar) {

		return Vector2(other.x - scalar.x, other.y - scalar.y);

	}
	inline Vector2& operator-(float scalar, const Vector2& other) {

		return Vector2(scalar - other.x, scalar - other.y);

	}
	inline Vector2& operator*(const Vector2& other, float scalar) {

		return Vector2(other.x * scalar, other.y * scalar);

	}
	inline Vector2& operator*(Vector2 const& other, Vector2 const& scalar) {

		return Vector2(other.x * scalar.x, other.y * scalar.y);

	}
	inline Vector2& operator*(float scalar, const Vector2& other) {

		return Vector2(scalar * other.x, scalar * other.y);

	}
	inline Vector2& operator/(const Vector2& other, float scalar) {

		return Vector2(other.x / scalar, other.y / scalar);

	}
	inline Vector2& operator/(const Vector2& other, const Vector2& scalar) {

		return Vector2(other.x / scalar.x, other.y / scalar.y);

	}
	inline Vector2& operator/(float scalar, const Vector2& other) {

		return Vector2(scalar / other.x, scalar / other.y);

	}

	inline Vector3& operator+(const Vector3& other, float scalar) {

		return Vector3(other.x + scalar, other.y + scalar, other.z + scalar);

	}
	inline Vector3& operator+(const Vector3& other, const Vector3& scalar) {

		return Vector3(other.x + scalar.x, other.y + scalar.y, other.z + scalar.z);

	}
	inline Vector3& operator+(float scalar, const Vector3& other) {

		return Vector3(scalar + other.x, scalar + other.y, scalar + other.z);

	}
	inline Vector3& operator-(const Vector3& other, float scalar) {

		return Vector3(other.x - scalar, other.y - scalar, other.z - scalar);

	}
	inline Vector3& operator-(const Vector3& other, const Vector3& scalar) {

		return Vector3(other.x - scalar.x, other.y - scalar.y, other.z - scalar.z);

	}
	inline Vector3& operator-(float scalar, const Vector3& other) {

		return Vector3(scalar - other.x, scalar - other.y, scalar - other.z);

	}
	inline Vector3& operator*(const Vector3& other, float scalar) {

		return Vector3(other.x * scalar, other.y * scalar, other.z * scalar);

	}
	inline Vector3& operator*(Vector3 const& other, Vector3 const& scalar) {

		return Vector3(other.x * scalar.x, other.y * scalar.y, other.z * scalar.z);

	}
	inline Vector3& operator*(float scalar, const Vector3& other) {

		return Vector3(scalar * other.x, scalar * other.y, scalar * other.z);

	}
	inline Vector3& operator/(const Vector3& other, float scalar) {

		return Vector3(other.x / scalar, other.y / scalar, other.z / scalar);

	}
	inline Vector3& operator/(const Vector3& other, const Vector3& scalar) {

		return Vector3(other.x / scalar.x, other.y / scalar.y, other.z / scalar.z);

	}
	inline Vector3& operator/(float scalar, const Vector3& other) {

		return Vector3(scalar / other.x, scalar / other.y, scalar / other.z);

	}

	inline Vector4& operator+(const Vector4& other, float scalar) {

		return Vector4(other.x + scalar, other.y + scalar, other.z + scalar, other.w + scalar);

	}
	inline Vector4& operator+(const Vector4& other, const Vector4& scalar) {

		return Vector4(other.x + scalar.x, other.y + scalar.y, other.z + scalar.z, other.w + scalar.w);

	}
	inline Vector4& operator+(float scalar, const Vector4& other) {

		return Vector4(scalar + other.x, scalar + other.y, scalar + other.z, scalar + other.w);

	}
	inline Vector4& operator-(const Vector4& other, float scalar) {

		return Vector4(other.x - scalar, other.y - scalar, other.z - scalar, other.w - scalar);

	}
	inline Vector4& operator-(const Vector4& other, const Vector4& scalar) {

		return Vector4(other.x - scalar.x, other.y - scalar.y, other.z - scalar.z, other.w - scalar.w);

	}
	inline Vector4& operator-(float scalar, const Vector4& other) {

		return Vector4(scalar - other.x, scalar - other.y, scalar - other.z, scalar - other.w);

	}
	inline Vector4& operator*(const Vector4& other, float scalar) {

		return Vector4(other.x * scalar, other.y * scalar, other.z * scalar, other.w * scalar);

	}
	inline Vector4& operator*(Vector4 const& other, Vector4 const& scalar) {

		return Vector4(other.x * scalar.x, other.y * scalar.y, other.z * scalar.z, other.w * scalar.w);

	}
	inline Vector4& operator*(float scalar, const Vector4& other) {

		return Vector4(scalar * other.x, scalar * other.y, scalar * other.z, scalar * other.w);

	}
	inline Vector4& operator/(const Vector4& other, float scalar) {

		return Vector4(other.x / scalar, other.y / scalar, other.z / scalar, other.w / scalar);

	}
	inline Vector4& operator/(const Vector4& other, const Vector4& scalar) {

		return Vector4(other.x / scalar.x, other.y / scalar.y, other.z / scalar.z, other.w / scalar.w);

	}
	inline Vector4& operator/(float scalar, const Vector4& other) {

		return Vector4(scalar / other.x, scalar / other.y, scalar / other.z, scalar / other.w);

	}

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

		return os << "R: " << col.r << ", G: " << col.g << ", B: " << col.b;

	}

}