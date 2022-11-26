#pragma once

#include "Engine/Core/Core.h"

#include "Engine/Scripting/Variable.h"

extern "C" {

	typedef struct _MonoClass MonoClass;
	typedef struct _MonoObject MonoObject;
	typedef struct _MonoMethod MonoMethod;
	typedef struct _MonoException MonoException;

}

namespace Copper {

	class Object;

	class ScriptComponent {

	public:
		ScriptComponent(std::string nameSpace, std::string name, MonoClass* klass) : nameSpace(nameSpace), scriptName(name), klass(klass) {}
		ScriptComponent(std::string nameSpace, std::string name, Object& obj);

		std::string nameSpace;
		std::string scriptName;
		MonoClass* klass;
		bool valid;

		void OnCreate();
		void OnUpdate();

		MonoObject* GetInstance() { return instance; }

	private:
		MonoObject* instance;

		MonoMethod* onCreate;
		void (*onUpdate) (MonoObject* obj, MonoException** exc);

	};

}