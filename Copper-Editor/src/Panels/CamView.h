#pragma once

#include <Copper.h>

#include "Panels/Panel.h"

namespace Editor {

    using namespace Copper;

    class CamView : public Panel {

    public:
        CamView() : Panel("Cam View") {}

        virtual void UIRender() override;

        inline bool IsAcceptingInput() { return m_isAcceptingInput; }
        inline void SetIsAcceptingInput(bool value) { m_isAcceptingInput = value; }

    private:
        bool m_isAcceptingInput = false;

        virtual void UI() override {}

    };

}
