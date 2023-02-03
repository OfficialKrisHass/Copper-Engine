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

	Color Color::red			= Color(1.0f, 0.0f, 0.0f);
	Color Color::green			= Color(0.0f, 1.0f, 0.0f);
	Color Color::blue			= Color(0.0f, 0.0f, 1.0f);
	Color Color::white			= Color(1.0f, 1.0f, 1.0f);
	Color Color::black			= Color(0.0f, 0.0f, 0.0f);
	Color Color::transparent	= Color(1.0f, 1.0f, 1.0f, 0.0f);

	Vector2::operator UVector2I() const { return UVector2I((uint32_t) x, (uint32_t) y); }
	Vector2::operator Vector2I() const { return Vector2I((int32_t) x, (int32_t) y); }

	UVector2I::operator Vector2() const { return Vector2((float) x, (float) y); }
	Vector2I::operator Vector2() const { return Vector2((float) x, (float) y); }

}