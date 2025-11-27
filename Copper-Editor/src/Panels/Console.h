#pragma once

#include <Copper.h>

#include "Panels/Panel.h"

namespace Editor {

    // TODO: Actually implement this you fukctard ??

    class Console : public Panel {

    public:
        Console() : Panel("Console") {}

        void Initialize();

        static inline void Clear() { m_messages.clear(); }
        
    private:
        struct Message {

            std::string text;
            spdlog::level::level_enum level;

        };

        static std::vector<Message> m_messages;

        virtual void UI() override;

        static void LogCallback(const spdlog::details::log_msg& msg);

    };

}
