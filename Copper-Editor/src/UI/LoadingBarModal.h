#pragma once

#include <Copper.h>

namespace Editor {

    class LoadingBar {

    public:
        LoadingBar() = default;
        LoadingBar(const std::string& title) : m_title(title) {}

        void DrawUI();

        void SetStatus(float value, const std::string& msg) {

            CUP_FUNCTION();

            m_status = value;
            m_statusMsg = msg;

        }

    private:
        std::string m_title;
        bool m_open = false;

        float m_status = 0.0f;
        std::string m_statusMsg;

        void Open();

    };

}
