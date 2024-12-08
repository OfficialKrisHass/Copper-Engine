#pragma once

#include "Engine/Core/Core.h"
#include "Engine/Core/Window.h"

struct ImGuiContext;

namespace Copper {

    class UIContext {

    public:
        UIContext() = default;

        void Initialize(const Window& window, bool gizmo = false, bool docking = true, bool viewports = true);
        void Shutdown();

        void Begin();
        void End();

        void LoadFont(const std::string& path, float fontSize = 18.0f) const;

        void SetAsCurrent() const;
    
    private:
        bool m_gizmo = false;
        bool m_docking = true;
        bool m_viewports = true;

        ImGuiContext* m_context = nullptr;

    };

    const UIContext& MainUIContext();

}
