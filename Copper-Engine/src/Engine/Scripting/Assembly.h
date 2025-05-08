#pragma once

#include "Engine/Core/Core.h"

extern "C" {

    typedef struct _MonoAssembly MonoAssembly;
    typedef struct _MonoImage MonoImage;

}

namespace Copper::Scripting {

    class Script;

    struct Assembly {

        friend Script;

        Assembly() = default;
        Assembly(const fs::path& path) { Create(path); }

        void Create(const fs::path& path);
        
        inline const fs::path& Path() const { return m_path; }

        inline MonoAssembly* GetAssembly() const { return m_assembly; }
        inline MonoImage* GetImage() const { return m_image; }

        inline bool IsValid() const { return m_assembly != nullptr; }

    private:
        fs::path m_path = "";

        MonoAssembly* m_assembly = nullptr;
        MonoImage* m_image = nullptr;

        void OpenImage(const fs::path& path);
        void LoadAssembly(const fs::path& path);

    };

}
