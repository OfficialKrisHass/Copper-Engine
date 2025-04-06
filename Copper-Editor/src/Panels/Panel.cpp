#include "Panel.h"

#include <ImGui/imgui.h>

namespace Editor {

    using namespace Copper;

    void Panel::UIRender() {

        CUP_FUNCTION();

        ImGui::Begin(m_name.c_str());

        m_focused = ImGui::IsWindowFocused();
        UI();

        ImGui::End();

    }

}
