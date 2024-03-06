#pragma once

#include <Copper.h>

#define DragIntSpeed 1.0f
#define DragFloatSpeed 0.01f

namespace Editor::UI {

	bool EditBool(const std::string& name, bool* show);
	bool EditInt(const std::string& name, Copper::int32* show);
	bool EditUInt(const std::string& name, Copper::uint32* show);
	bool EditFloat(const std::string& name, float* show);
	bool EditDouble(const std::string& name, double* show);
	bool EditString(const std::string& name, std::string* show);
	bool EditChar(const std::string& name, char* show);

	bool EditVector2(const std::string& name, Copper::Vector2* vec);
	bool EditVector3(const std::string& name, Copper::Vector3* vec);
	bool EditVector4(const std::string& name, Copper::Vector4* vec);
	bool EditColor(const std::string& name, Copper::Color* col);

	bool EditEntity(const std::string& name, Copper::InternalEntity** entity);
	bool EditTransform(const std::string& name, Copper::Transform** transform);

	bool EditTexture(const std::string& name, Copper::Texture* texture);

	bool EditDropDown(const std::string& name, const char* items[], Copper::uint32 count, Copper::uint32* selected);
	bool EditMask(const std::string& name, Copper::uint32& mask, Copper::uint32 num, Copper::uint32 maskOffset = 0, char startLabel = 'X');

}