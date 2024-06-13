#include "Base.h"
#include "Utils.h"

#include <mono/jit/jit.h>

#include <mono/metadata/assembly.h>
#include <mono/metadata/mono-config.h>

#include <iostream>

namespace APIBinder {

    MonoDomain* domain = nullptr;

    MonoImage* image = nullptr;
    MonoAssembly* assembly = nullptr;

    void InitMono();
    void LoadAssembly();

    int Entry() {

        InitMono();
        LoadAssembly();

        return 0;

    }

    void InitMono() {

        mono_config_parse("Copper-Editor/lib/mono/config");
        mono_set_assemblies_path("Copper-Editor/lib");

        domain = mono_jit_init("APIBinderDomain");
        if (domain != nullptr) return;

        std::cerr << "Could not initiaize mono JIT\n";
        exit(1);

    }
    void LoadAssembly() {

        uint32 size = 0;
        char* data = ReadFileBinary("Copper-Editor/assets/ScriptingAPI/Copper-ScriptingAPI.dll", &size);
        if (!data) {

            std::cerr << "Could not read Assembly!\n";
            exit(1);

        }

        MonoImageOpenStatus status;
        image = mono_image_open_from_data_full(data, size, true, &status, false);
        delete[] data;
        if (status != MONO_IMAGE_OK) {

            std::cerr << "Failed to open assembly Image.\n\tMessage: " << mono_image_strerror(status) << "\n";
            exit(1);

        }

        assembly = mono_assembly_load_from_full(image, "Copper-Editor/assets/ScriptingAPI/Copper-ScriptingAPI.dll", &status, false);
        mono_image_close(image);

        if (assembly == nullptr) {

            std::cerr << "Failed to load assembly from image.\n\tMessage: " << mono_image_strerror(status) << "\n";
            exit(1);

        }

        image = mono_assembly_get_image(assembly);
        std::cout << "Assembly Loaded successfully!\n";
        
    }

}

int main() { return APIBinder::Entry(); }
