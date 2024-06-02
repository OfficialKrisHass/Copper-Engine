#include "cupch.h"
#include "ScriptingEngine.h"

#include "Engine/Core/Engine.h"

#include "Engine/Scene/Scene.h"
#include "Engine/Scene/ComponentView.h"

#include "Engine/Scripting/Script.h"
#include "Engine/Scripting/Field.h"
#include "Engine/Scripting/Classes.h"

#include "Engine/Components/ScriptComponent.h"

#include "Engine/Input/Popup.h"

#include <mono/jit/jit.h>
#include <mono/metadata/class.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/mono-config.h>

namespace Copper::Scripting {

    static char AppDomainName[] = "CUSAppDomain";

    struct ScriptingData {

        MonoDomain* rootDomain = nullptr;
        MonoDomain* appDomain = nullptr;

        Assembly scriptingAPI;
        Assembly game;

        MonoClassField* unmanagedPtrField;

        ScriptMap componentScripts;

    };
    ScriptingData data;

    void InitializeScriptingAPI();
    void InitializeGame();

    extern void SetupInternalCalls();
    extern void InitializeClasses();

    void Initialize() {

        CUP_FUNCTION();
        VERIFY_STATE(EngineCore::EngineState::Initialization, "Initialize the Scripting Engine");

        mono_config_parse((ExecutableFolder() + "/lib/mono/config").c_str());
        mono_set_assemblies_path((ExecutableFolder() + "/lib").c_str());
        data.rootDomain = mono_jit_init("CUSRootDomain");
        if (!data.rootDomain) {

            Input::ErrorPopup("JIT initialization failed", "Failed to initialize the JIT mono runtime.");
            exit(1);

        }

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

        data.componentScripts.clear();

    }
    bool Reload() {

        CUP_FUNCTION();

        std::string tmp = data.game.Path();

        std::unordered_map<ScriptComponent*, std::string> scriptComponentNames;
        for (ScriptComponent* scriptComponent : ComponentView<ScriptComponent>(GetScene()))
            scriptComponentNames[scriptComponent] = scriptComponent->GetScript()->FullName();

        Unload();

        InitializeScriptingAPI();
        if (!Load(tmp)) {

            LogError("Failed to load game assembly at path '{}'", tmp);
            return false;

        }

        for (auto it = scriptComponentNames.begin(); it != scriptComponentNames.end(); ++it) {

            if (data.componentScripts.find(it->second) != data.componentScripts.end())
                it->first->Setup(&data.componentScripts[it->second]);

            LogError("Script '{}' is missing for Script Component on Entity '{}'", it->second, *it->first->GetEntity());

        }

        return true;

    }

    void InitializeScriptingAPI() {

        CUP_FUNCTION();

        data.appDomain = mono_domain_create_appdomain(AppDomainName, nullptr);
        mono_domain_set(data.appDomain, true);

        // I forgot I changed the dir name from ScriptAPI to Script - ing - API only here and didnt change the
        // Scripting api build directory and spent 2 days trying to figure out why the fuck nothing was working
        data.scriptingAPI = Assembly(ExecutableFolder() + "/assets/ScriptingAPI/Copper-ScriptingAPI.dll");

        SetupInternalCalls();
        InitializeClasses();

        data.unmanagedPtrField = mono_class_get_field_from_name(BaseClass(), "m_unmanagedPtr");

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

            MonoClass* klass = mono_class_from_name_case(data.game.GetImage(), nameSpace, name);
            if (!mono_class_is_subclass_of(klass, ComponentClass(), false)) continue;

            std::string fullName = mono_class_get_name(klass);
            Script& script = data.componentScripts[fullName] = Script(klass);

        }

    }

    MonoDomain* AppDomain() { return data.appDomain; }

    const Assembly& ScriptingAPIAssembly() { return data.scriptingAPI; }
    const Assembly& GameAssembly() { return data.game; }

    MonoClassField* UnmanagedPtrField() { return data.unmanagedPtrField; }

    const ScriptMap& ComponentScripts() { return data.componentScripts; }

}
