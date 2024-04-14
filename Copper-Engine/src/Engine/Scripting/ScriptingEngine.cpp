#include "cupch.h"
#include "ScriptingEngine.h"

#include "Engine/Core/Engine.h"

#include "Engine/Scripting/Script.h"

#include "Engine/Input/Popup.h"

#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>

namespace Copper::Scripting {

    struct ScriptingData {

        MonoDomain* rootDomain = nullptr;
        MonoDomain* appDomain = nullptr;

        Assembly scriptingAPI;
        Assembly game;

        Script baseComponent;

        std::vector<Script> scriptComponents;

    };
    ScriptingData data;

    void InitializeScriptingAPI();
    void InitializeGame();

    extern void SetupInternalCalls();

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

        InitializeScriptingAPI();
        
    }
    void Shutdown() {

        CUP_FUNCTION();
        VERIFY_STATE(EngineCore::EngineState::Shutdown, "Shutdown the Scripting Engine");

        mono_jit_cleanup(data.rootDomain);

    }

    bool Load(const std::string& path) {

        CUP_FUNCTION();

        data.game = Assembly(path);
        if (!data.game) return false;

        InitializeGame();

         return true;

    }
    void Unload() {

        CUP_FUNCTION();

        mono_domain_set(data.rootDomain, false);
        mono_domain_unload(data.appDomain);

        data.game = Assembly();

        data.scriptComponents.clear();

    }
    bool Reload() {

        CUP_FUNCTION();

        std::string tmp = data.game.Path();

        Unload();

        InitializeScriptingAPI();
        return Load(tmp);

    }

    void InitializeScriptingAPI() {

        CUP_FUNCTION();

        data.scriptingAPI = Assembly(ExecutableFolder() + "/assets/ScriptingAPI/Copper-ScriptingAPI.dll");
        data.baseComponent = Script("Copper", "Component", data.scriptingAPI);

        SetupInternalCalls();

    }
    void InitializeGame() {

        CUP_FUNCTION();

        // Get the type definitions table

        const MonoTableInfo* typeTable = mono_image_get_table_info(data.game.GetImage(), MONO_TABLE_TYPEDEF);
        uint32 num = mono_table_info_get_rows(typeTable);

        for (uint32 i = 0; i < num; i++) {

            // Get type info

            uint32 cols[MONO_TYPEDEF_SIZE];
            mono_metadata_decode_row(typeTable, i, cols, MONO_TYPEDEF_SIZE);
            
            const char* nameSpace = mono_metadata_string_heap(data.game.GetImage(), cols[MONO_TYPEDEF_NAMESPACE]);
            const char* name = mono_metadata_string_heap(data.game.GetImage(), cols[MONO_TYPEDEF_NAME]);

            if (std::string(name) == "<Module>") continue;

            // Filter out non component Scripts

            data.scriptComponents.push_back(Script(nameSpace, name, data.game));
            Script& script = data.scriptComponents.back();

            MonoObject* instance = mono_object_new(data.appDomain, script.GetClass());
            CU_ASSERT(instance, "Could not instantiate scriptt class");

            mono_runtime_object_init(instance);

            MonoMethod* method = mono_class_get_method_from_name(script.GetClass(), "Print", 0);
            CU_ASSERT(method, "Could not Get method");

            MonoObject* exception = nullptr;
            mono_runtime_invoke(method, instance, nullptr, &exception);

            if (exception) {

                MonoClass* excClass = mono_object_get_class(exception);

                MonoString* str = nullptr;
                MonoProperty* prop = mono_class_get_property_from_name(excClass, "Message");
                str = (MonoString*) mono_runtime_invoke(mono_property_get_get_method(prop), exception, nullptr, nullptr);

                MonoError error;
                char* utf8 = mono_string_to_utf8(str);

                LogError(std::string(utf8));
                mono_free(utf8);

            }

            if (script.IsSubclassOf(data.baseComponent)) continue;
            data.scriptComponents.pop_back();

        }

    }

    MonoDomain* AppDomain() { return data.appDomain; }

    const Assembly& ScriptingAPIAssembly() { return data.scriptingAPI; }
    const Assembly& GameAssembly() { return data.game; }

    const Script& BaseComponent() { return data.baseComponent; }

    const std::vector<Script>& ScriptComponents() { return data.scriptComponents; }

}
