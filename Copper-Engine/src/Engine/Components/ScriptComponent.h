#pragma once

#include "Engine/Core/Core.h"

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

		void Init(int32_t objID, const std::string& name);

		std::string name = "";
		
		void InvokeCreate();
		void InvokeUpdate();

	private:
		MonoObject* instance;

		MonoMethod* create;
		void (*update) (MonoObject* obj, MonoException** exc);

	};

}