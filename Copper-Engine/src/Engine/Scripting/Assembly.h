#pragma once

#include "Engine/Core/Core.h"

extern "C" {

    typedef struct _MonoAssembly MonoAssembly;
    typedef struct _MonoImage MonoImage;

}

namespace Copper::Scripting {

    struct Assembly {

        Assembly() = default;
        Assembly(const std::string& path);

    private:
        MonoAssembly* m_assembly = nullptr;
        MonoImage* m_image = nullptr;

        void OpenImage(const std::string& path);
        void LoadAssembly(const std::string& path);

    };

}
