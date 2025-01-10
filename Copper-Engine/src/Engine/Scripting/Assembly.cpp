#include "cupch.h"
#include "Assembly.h"

#include "Engine/Utilities/FileUtils.h"

#include <mono/metadata/assembly.h>

namespace Copper::Scripting {

    Assembly::Assembly(const fs::path& path) {

        CUP_FUNCTION();

        m_path = path;

        OpenImage(path);
        LoadAssembly(path);

        if (!m_assembly) return;
        m_image = mono_assembly_get_image(m_assembly);

    }

    void Assembly::OpenImage(const fs::path& path) {

        CUP_FUNCTION();

        uint32 dataSize = 0;
        char* data = Utilities::ReadFileBinary(path, &dataSize);
        if (!data) {

            LogError("Could not read assembly.\n\tPath: {}", path);
            return;

        }

        MonoImageOpenStatus status;
        m_image = mono_image_open_from_data_full(data, dataSize, true, &status, false);
        if (status != MONO_IMAGE_OK) {

            LogError("Failed to open the assembly image.\n\tPath: {}\n\tError message: {}", path, mono_image_strerror(status));

            delete[] data;
            return;

        }

        delete[] data;


    }
    void Assembly::LoadAssembly(const fs::path& path) {

        CUP_FUNCTION();

        if (!m_image) return;

        MonoImageOpenStatus status;
        m_assembly = mono_assembly_load_from_full(m_image, path.string().c_str(), &status, false);
        mono_image_close(m_image);
        
        if (m_assembly) return;
        LogError("Failed to load the assembly from image.\n\tPath: {}\n\tError message: {}", path, mono_image_strerror(status));

    }

}
