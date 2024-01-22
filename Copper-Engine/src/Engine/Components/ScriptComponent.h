#pragma once

#include "Engine/Core/Core.h"

#include "Engine/Components/Component.h"

#include "Engine/Scripting/Field.h"

extern "C" {

	typedef struct _MonoClass MonoClass;
	typedef struct _MonoObject MonoObject;
	typedef struct _MonoMethod MonoMethod;
	typedef struct _MonoException MonoException;

}

namespace Copper {

	typedef void (*UpdateFunc)(MonoObject* obj, MonoException** exc);
	typedef void (*OnCollisionPersistFunc)(MonoObject* obj, MonoObject* other, MonoException** exc);

	class ScriptComponent : public Component {

	public:
		ScriptComponent() = default;

		std::string name = "";

		void Init(const std::string& name);

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
		MonoObject* m_instance = nullptr;

		MonoMethod* m_create = nullptr;
		UpdateFunc m_update = nullptr;

		// Collision Eventss

		MonoMethod* m_onCollisionBegin = nullptr;
		OnCollisionPersistFunc m_onCollisionPersist = nullptr;
		MonoMethod* m_onCollisionEnd = nullptr;

		// Trigger Events

		MonoMethod* m_onTriggerEnter = nullptr;
		MonoMethod* m_onTriggerLeave = nullptr;

		void Invalidate();

	};

}