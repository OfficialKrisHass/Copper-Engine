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
        Assembly(const std::string& path);
        
        inline const std::string& Path() const { return m_path; }

        inline MonoAssembly* GetAssembly() const { return m_assembly; }
        inline MonoImage* GetImage() const { return m_image; }

        inline operator bool() const { return m_assembly != nullptr; }

    private:
        std::string m_path = "";

        MonoAssembly* m_assembly = nullptr;
        MonoImage* m_image = nullptr;

        void OpenImage(const std::string& path);
        void LoadAssembly(const std::string& path);

    };

}
