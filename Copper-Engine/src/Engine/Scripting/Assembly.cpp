#include "cupch.h"
#include "Assembly.h"

#include "Engine/Utilities/FileUtils.h"

#include <mono/metadata/assembly.h>
#include <mono/metadata/appdomain.h>

namespace Copper::Scripting {

    void Assembly::Create(const fs::path& path) {

        CUP_FUNCTION();

        m_path = path;

        OpenImage();
        if (m_image == nullptr) return;

        MonoImageOpenStatus status;
        m_assembly = mono_assembly_load_from_full(m_image, path.string().c_str(), &status, false);
        if (status != MONO_IMAGE_OK) {

            LogError("Failed to load assembly from image. Assembly: '{}', Error: {}", path.string(), mono_image_strerror(status));
            return;

        }

        m_valid = true;

    }
    void Assembly::Unload() {

        CUP_FUNCTION();

        m_valid = false;

    }

    void Assembly::OpenImage() {

        CUP_FUNCTION();

        m_image = nullptr;

        uint32 dataSize = 0;
        char* data = Utilities::ReadFileBinary(m_path, &dataSize);
        if (data == nullptr) {

            LogError("Could not read assembly '{}'", m_path.string());
            return;

        }

        MonoImageOpenStatus status;
        m_image = mono_image_open_from_data_full(data, dataSize, true, &status, false);

        if (status != MONO_IMAGE_OK) {

            LogError("Could not open assembly image. Assembly '{}', Error: {}", m_path.string(), mono_image_strerror(status));
            delete[] data;

            return;

        }

        delete[] data;


    }

}
