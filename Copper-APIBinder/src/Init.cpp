#include "Init.h"
#include "Utils.h"

#include <mono/jit/jit.h>

#include <mono/metadata/assembly.h>
#include <mono/metadata/mono-config.h>

#include <iostream>

namespace APIBinder {

    MonoDomain* InitMono() {

        std::cout << "Initializing Mono\n";

        mono_config_parse("Copper-Editor/lib/mono/config");
        mono_set_assemblies_path("Copper-Editor/lib");

        MonoDomain* ret = mono_jit_init("APIBinderDomain");
        if (ret == nullptr) {

            std::cerr << "Could not initiaize mono JIT\n";
            exit(1);

        }

        std::cout << "Successfully initialized Mono\n";
        return ret;

    }
    MonoAssembly* LoadAssembly(MonoImage** image) {

        std::cout << "Loading Assembly at path: Copper-Editor/assets/ScriptingAPI/Copper-ScriptingAPI.dll\n";

        uint32 size = 0;
        char* data = ReadFileBinary("Copper-Editor/assets/ScriptingAPI/Copper-ScriptingAPI.dll", &size);
        if (!data) {

            std::cerr << "Could not read Assembly!\n";
            exit(1);

        }

        MonoImageOpenStatus status;
        *image = mono_image_open_from_data_full(data, size, true, &status, false);
        delete[] data;
        if (status != MONO_IMAGE_OK) {

            std::cerr << "Failed to open assembly Image.\n\tMessage: " << mono_image_strerror(status) << "\n";
            exit(1);

        }

        MonoAssembly* ret = mono_assembly_load_from_full(*image, "Copper-Editor/assets/ScriptingAPI/Copper-ScriptingAPI.dll", &status, false);
        mono_image_close(*image);

        if (ret == nullptr) {

            std::cerr << "Failed to load assembly from image.\n\tMessage: " << mono_image_strerror(status) << "\n";
            exit(1);

        }

        *image = mono_assembly_get_image(ret);
        std::cout << "Assembly loaded successfully!\n";

        return ret;

    }

}
