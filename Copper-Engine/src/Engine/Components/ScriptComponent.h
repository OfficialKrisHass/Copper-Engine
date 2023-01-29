#pragma once

#include "Engine/Core/Core.h"

#include "Engine/Scripting/Field.h"

#include <CopperECS/Component.h>

extern "C" {

	typedef struct _MonoClass MonoClass;
	typedef struct _MonoObject MonoObject;
	typedef struct _MonoMethod MonoMethod;
	typedef struct _MonoException MonoException;

}

namespace Copper {

	class ScriptComponent : public Component {

	public:
		ScriptComponent() = default;

		std::string name = "";

		void Init(int32_t objID, const std::string& name);

		void CopyTo(MonoObject* other);
		
		void InvokeCreate();
		void InvokeUpdate();

		void GetFieldValue(const ScriptField& field, void* out);
		void GetFieldValue(const ScriptField& field, Object* out);
		void GetFieldValue(const ScriptField& field, Component* out);

		void SetFieldValue(const ScriptField& field, void* value);
		void SetFieldValue(const ScriptField& field, Object* value);
		void SetFieldValue(const ScriptField& field, Component* value);

	private:
		MonoObject* instance = nullptr;

		MonoMethod* create = nullptr;
		void (*update) (MonoObject* obj, MonoException** exc) = nullptr;

	};

}