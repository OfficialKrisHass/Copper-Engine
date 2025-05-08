#include "cupch.h"
#include "Assembly.h"

#include <mono/metadata/assembly.h>
#include <mono/metadata/appdomain.h>

namespace Copper::Scripting {

    void Assembly::Create(const fs::path& path) {

        CUP_FUNCTION();

        m_path = path;

        m_assembly = mono_domain_assembly_open(mono_domain_get(), path.string().c_str());
        CU_ASSERT(m_assembly != nullptr, "Could not open assembly at {}", path);

        m_image = mono_assembly_get_image(m_assembly);
        CU_ASSERT(m_image != nullptr, "Could not get assembly image from assembly at {}", path);

        m_valid = true;

    }
    void Assembly::Unload() {

        CUP_FUNCTION();

        m_valid = false;

    }

}
