#pragma once

#include "Engine/Core/Core.h"

#include "Engine/Scripting/Field.h"

#include "Engine/Scene/Component.h"

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

		void Init(const std::string& name);
		//void Init(uint32_t objID, const std::string& name);

		void CopyTo(MonoObject* other);
		
		void InvokeCreate();
		void InvokeUpdate();

		void InvokeOnCollisionBegin(InternalEntity* other);
		void InvokeOnCollisionPersist(InternalEntity* other);
		void InvokeOnCollisionEnd(InternalEntity* other);

		void InvokeOnTriggerEnter(InternalEntity* other);
		void InvokeOnTriggerLeave(InternalEntity* other);

		void GetFieldValue(const ScriptField& field, void* out);
		void GetFieldValue(const ScriptField& field, InternalEntity** out);
		void GetFieldValue(const ScriptField& field, Transform** out);

		void SetFieldValue(const ScriptField& field, void* value);
		void SetFieldValue(const ScriptField& field, InternalEntity** value);
		void SetFieldValue(const ScriptField& field, Transform** value);

	private:
		MonoObject* instance = nullptr;

		MonoMethod* create = nullptr;
		void (*update) (MonoObject* obj, MonoException** exc) = nullptr;

		// Collision Eventss

		MonoMethod* onCollisionBegin = nullptr;
		void (*onCollisionPersist) (MonoObject* obj, MonoObject* other, MonoException** exc) = nullptr;
		MonoMethod* onCollisionEnd = nullptr;

		// Trigger Events

		MonoMethod* onTriggerEnter = nullptr;
		MonoMethod* onTriggerLeave = nullptr;

		void Invalidate();

	};

}