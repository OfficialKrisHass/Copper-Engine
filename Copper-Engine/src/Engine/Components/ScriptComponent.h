#pragma once

#include "Engine/Core/Core.h"

extern "C" {

	typedef struct _MonoObject MonoObject;
	typedef struct _MonoMethod MonoMethod;

}

namespace Copper {

	class Object;

	class ScriptComponent {

	public:
		ScriptComponent(std::string nameSpace, std::string name, Object& obj);

		std::string name;
		bool valid;

		void OnCreate();
		void OnUpdate();

	private:
		MonoObject* instance;

		MonoMethod* onCreate;
		MonoMethod* onUpdate;

	};

}