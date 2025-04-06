#pragma once

#include "Copper.h"

namespace Editor {

    class Panel {

    public:
        Panel() = default;
        Panel(const std::string& name) : m_name(name) {}

        void UIRender();

        inline bool IsFocused() const { return m_focused; }

    private:
        std::string m_name;

        bool m_focused = false;

        virtual void UI() = 0;

    };

}
