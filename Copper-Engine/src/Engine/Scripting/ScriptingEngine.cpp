#include "cupch.h"
#include "ScriptingEngine.h"

#include "Engine/Core/Engine.h"

#include "Engine/Input/Popup.h"

#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>

namespace Copper::Scripting {

    struct ScriptingData {

        MonoDomain* rootDomain = nullptr;
        MonoDomain* appDomain = nullptr;

        Assembly scriptingAPI;

    };
    ScriptingData data;

    void Initialize() {

        CUP_FUNCTION();
        VERIFY_STATE(EngineCore::EngineState::Initialization, "Initialize the Scripting Engine");

        mono_set_assemblies_path((ExecutableFolder() + "/lib").c_str());
        data.rootDomain = mono_jit_init("CUSRootDomain");
        if (!data.rootDomain) {

            Input::ErrorPopup("JIT initialization failed", "Failed to initialize the JIT mono runtime.");
            exit(1);

        }

        data.appDomain = mono_domain_create_appdomain("CUSAppDomain", nullptr);
        mono_domain_set(data.appDomain, true);

        data.scriptingAPI = Assembly(ExecutableFolder() + "/assets/ScriptingAPI/Copper-ScriptingAPI.dll");

    }
    void Shutdown() {

        mono_jit_cleanup(data.rootDomain);

    }

    const Assembly& ScriptingAPIAssembly() { return data.scriptingAPI; }

}
