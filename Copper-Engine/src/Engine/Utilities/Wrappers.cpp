#include "cupch.h"
#include "Wrappers.h"

namespace Copper {

	Vector2 Vector2::zero		= Vector2(0.0f);
	Vector2 Vector2::one		= Vector2(1.0f);
	Vector2 Vector2::minusOne	= Vector2(-1.0f);

	Vector3 Vector3::zero		= Vector3(0.0f);
	Vector3 Vector3::one		= Vector3(1.0f);
	Vector3 Vector3::minusOne	= Vector3(-1.0f);

	Vector4 Vector4::zero		= Vector4(0.0f);
	Vector4 Vector4::one		= Vector4(1.0f);
	Vector4 Vector4::minusOne	= Vector4(-1.0f);

	Vector2I Vector2I::zero	= Vector2I(0);
	Vector2I Vector2I::one	= Vector2I(0);
	Vector2I Vector2I::minusOne	= Vector2I(-1);

	UVector2I UVector2I::zero	= UVector2I(0);
	UVector2I UVector2I::one	= UVector2I(1);

	Vector3I Vector3I::zero = Vector3I(0);
	Vector3I Vector3I::one = Vector3I(0);
	Vector3I Vector3I::minusOne = Vector3I(-1);

	UVector3I UVector3I::zero = UVector3I(0);
	UVector3I UVector3I::one = UVector3I(1);

	Vector4I Vector4I::zero = Vector4I(0);
	Vector4I Vector4I::one = Vector4I(0);
	Vector4I Vector4I::minusOne = Vector4I(-1);

	UVector4I UVector4I::zero = UVector4I(0);
	UVector4I UVector4I::one = UVector4I(1);

	Color Color::red			= Color(1.0f, 0.0f, 0.0f);
	Color Color::green			= Color(0.0f, 1.0f, 0.0f);
	Color Color::blue			= Color(0.0f, 0.0f, 1.0f);
	Color Color::white			= Color(1.0f, 1.0f, 1.0f);
	Color Color::black			= Color(0.0f, 0.0f, 0.0f);
	Color Color::transparent	= Color(1.0f, 1.0f, 1.0f, 0.0f);

	Vector2::operator Vector2I() const { return Vector2I((int32_t) x, (int32_t) y); }
	Vector2::operator UVector2I() const { return UVector2I((uint32_t) x, (uint32_t) y); }
	Vector2I::operator UVector2I() const { return UVector2I((uint32_t) x, (uint32_t) y); }

	Vector3::operator Vector3I() const { return Vector3I((int32_t) x, (int32_t) y, (int32_t) z); }
	Vector3::operator UVector3I() const { return UVector3I((uint32_t) x, (uint32_t) y, (uint32_t) z); }
	Vector3I::operator UVector3I() const { return UVector3I((uint32_t) x, (uint32_t) y, (uint32_t) z); }

	Vector4::operator Vector4I() const { return Vector4I((int32_t) x, (int32_t) y, (int32_t) z, (int32_t) w); }
	Vector4::operator UVector4I() const { return UVector4I((uint32_t) x, (uint32_t) y, (uint32_t) z, (uint32_t) w); }
	Vector4I::operator UVector4I() const { return UVector4I((uint32_t) x, (uint32_t) y, (uint32_t) z, (uint32_t) w); }

	Quaternion Quaternion::identity = Quaternion(1.0f, Vector3::zero);

}