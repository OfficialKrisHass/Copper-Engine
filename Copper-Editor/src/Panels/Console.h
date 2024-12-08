#pragma once

#include <Copper.h>

#include "Panels/Panel.h"

namespace Editor {

    // TODO: Actually implement this you fukctard ??

    class Console : public Panel {

    public:
        Console() : Panel("Console") {}
        
    private:
        virtual void UI() override;

    };

}
