#include "cupch.h"
#include "ScriptingEngine.h"

#include "Engine/Core/Engine.h"

#include "Engine/Scene/Scene.h"
#include "Engine/Scene/ComponentView.h"

#include "Engine/Scripting/Script.h"
#include "Engine/Scripting/Field.h"
#include "Engine/Scripting/Classes.h"
#include "Engine/Scripting/ManagedReferences.h"

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
        VERIFY_STATE(EngineState::Initialization, "Initialize the Scripting Engine");

#ifdef CU_LINUX
        mono_set_assemblies_path((ExecutableFolder() / "lib/mono/lib/linux").string().c_str());
#elif CU_WINDOWS
        mono_set_assemblies_path((ExecutableFolder() / "lib/mono/lib/windows").string().c_str());
#endif

        mono_config_parse((ExecutableFolder() / "lib/mono/config").string().c_str());

        data.rootDomain = mono_jit_init("CUSRootDomain");
        if (!data.rootDomain) {

            Input::ErrorPopup("JIT initialization failed", "Failed to initialize the JIT mono runtime.");
            exit(1);

        }

        Log("\tScripting Engine initialized.");

        InitializeScriptingAPI();
        
    }
    void Shutdown() {

        CUP_FUNCTION();
        VERIFY_STATE(EngineState::Shutdown, "Shutdown the Scripting Engine");

        mono_jit_cleanup(data.rootDomain);

        Log("\tScripting engine shut down.");

    }

    bool Load(const fs::path& assemblyPath) {

        CUP_FUNCTION();

        if (!data.scriptingAPI)
            InitializeScriptingAPI();

        data.game = Assembly(assemblyPath);
        if (!data.game) return false;

        InitializeGame();

        Log("\tGame assembly '{}' loaded with {} component scripts.", assemblyPath.filename().string(), data.componentScripts.size());

        return true;

    }
    void Unload() {

        CUP_FUNCTION();

        mono_domain_set(data.rootDomain, false);
        mono_domain_unload(data.appDomain);

        ClearManagedReferences();

        data.game = Assembly();
        data.scriptingAPI = Assembly();

        data.componentScripts.clear();

        Log("\tGame assembly unloaded.");


    }
    bool Reload() {

        CUP_FUNCTION();

        if (!data.game) {

            LogError("Can't reload without a loaded game assembly");
            return false;

        }

        fs::path savedPath = data.game.Path();

        Unload();

        InitializeScriptingAPI();
        if (!Load(savedPath)) {

            LogError("Failed to load game assembly at path '{}'", savedPath);
            return false;

        }

        return true;

    }

    void InitializeScriptingAPI() {

        CUP_FUNCTION();

        data.appDomain = mono_domain_create_appdomain(AppDomainName, nullptr);
        mono_domain_set(data.appDomain, true);

        // I forgot I changed the dir name from ScriptAPI to Script - ING - API only here and didnt change the
        // Scripting api build directory and spent 2 days trying to figure out why the fuck nothing was working
        data.scriptingAPI = Assembly(ExecutableFolder() / "assets/ScriptingAPI/Copper-ScriptingAPI.dll");

        SetupInternalCalls();
        InitializeClasses();

        data.unmanagedPtrField = mono_class_get_field_from_name(BaseClass(), "m_unmanagedPtr");

        Log("\tScripting API initialized.");

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
            if (!mono_class_is_subclass_of(klass, GetMonoClass<Component>(), false)) continue;

            std::string fullName = mono_class_get_name(klass);
            data.componentScripts[fullName] = Script(klass);

        }

    }

    MonoDomain* AppDomain() { return data.appDomain; }

    const Assembly& ScriptingAPIAssembly() { return data.scriptingAPI; }
    const Assembly& GameAssembly() { return data.game; }

    const ScriptMap& ComponentScripts() { return data.componentScripts; }
    const Script* GetScript(const std::string& name) {

        CUP_FUNCTION();

        if (data.componentScripts.find(name) == data.componentScripts.end()) return nullptr;
        return &data.componentScripts.at(name);

    }

    MonoClassField* UnmanagedPtrField() { return data.unmanagedPtrField; }

}
