#include "cupch.h"
#include "Assembly.h"

#include "Engine/Utilities/FileUtils.h"

#include <mono/metadata/assembly.h>

namespace Copper::Scripting {

    void Assembly::Create(const fs::path& path) {

        CUP_FUNCTION();

        m_path = path;

        OpenImage(path);
        LoadAssembly(path);

        if (m_assembly != nullptr)
            m_image = mono_assembly_get_image(m_assembly);

    }

    void Assembly::OpenImage(const fs::path& path) {

        CUP_FUNCTION();

        uint32 dataSize = 0;
        char* data = Utilities::ReadFileBinary(path, &dataSize);
        if (!data) {

            LogError("Could not read assembly at {}.", path);
            return;

        }

        MonoImageOpenStatus status;
        m_image = mono_image_open_from_data_full(data, dataSize, true, &status, false);
        if (status != MONO_IMAGE_OK)
            LogError("Could not open assembly image at {}. Error: {}.", path, mono_image_strerror(status));

        delete[] data;

    }
    void Assembly::LoadAssembly(const fs::path& path) {

        CUP_FUNCTION();

        if (m_image == nullptr) return;

        MonoImageOpenStatus status;
        m_assembly = mono_assembly_load_from_full(m_image, path.string().c_str(), &status, false);
        
        if (m_assembly == nullptr)
            LogError("Failed to load assembly at {}. Error: {}.", path, mono_image_strerror(status));

        mono_image_close(m_image);

    }

}
