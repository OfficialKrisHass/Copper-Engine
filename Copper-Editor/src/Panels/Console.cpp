#include "Console.h"

#include "Engine/UI/ImGui.h"

#include <ImGui/imgui.h>

using namespace Copper;

namespace Editor {

    std::vector<Console::Message> Console::m_messages;

    ImFont* font = nullptr;

    void Console::Initialize() {

        CUP_FUNCTION();

        Logger::SetCallback(Console::LogCallback);

        font = GetMainUIContext().LoadFont(ExecutableFolder() / "assets/Fonts/IBMPlexMono-Medium.ttf", 16.0f);

    }

    void Console::UI() {

        if (ImGui::Button("Test"))
            Log("Test");
        
        ImGui::SameLine();
        if (ImGui::Button("Test warn"))
            LogWarn("Test warning");

        ImGui::SameLine();
        if (ImGui::Button("Test error"))
            LogError("Test error");

        ImGui::PushFont(font);

        for (const Message& msg : m_messages) {

            switch (msg.level) {

                case spdlog::level::info:
                case spdlog::level::trace: ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f)); break;
                case spdlog::level::warn: ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.9f, 0.4f, 1.0f)); break;
                case spdlog::level::err: ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.2f, 0.2f, 1.0f)); break;
                default: continue;
                
            }

            ImGui::Text(msg.text.c_str());
            ImGui::Separator();

            ImGui::PopStyleColor();

        }

        ImGui::PopFont();

    }

    void Console::LogCallback(const spdlog::details::log_msg& msg) {

        CUP_FUNCTION();

        if (msg.level == spdlog::level::debug) return;

        Message tmp;
        tmp.text = std::string(msg.payload.data(), msg.payload.size());
        tmp.level = msg.level;

        m_messages.push_back(std::move(tmp));

    }

}
