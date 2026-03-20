#include "Console.h"

#include "Engine/UI/ImGui.h"

#include <ImGui/imgui.h>

using namespace Copper;

namespace Editor {

    std::vector<Console::Message> Console::m_messages;

    ImFont* font = nullptr;

    bool showInfo = true;
    bool showWarn = true;
    bool showError = true;

    void Console::Initialize() {

        CUP_FUNCTION();

        Logger::SetCallback(Console::LogCallback);

        font = GetMainUIContext().LoadFont(ResourceDirectory() / "assets/Fonts/IBMPlexMono-Medium.ttf", 16.0f);

    }

    void Console::UI() {

        CUP_FUNCTION();

        if (ImGui::Button("Clear"))
            Clear();

        ImGui::SameLine();
        ImGui::Checkbox("Info", &showInfo);
        ImGui::SameLine();
        ImGui::Checkbox("Warn", &showWarn);
        ImGui::SameLine();
        ImGui::Checkbox("Error", &showError);

        ImGui::PushFont(font);

        for (const Message& msg : m_messages) {

            switch (msg.level) {

                case spdlog::level::info:
                case spdlog::level::trace: {

                    if (!showInfo) continue;
                    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));

                    break;

                }
                case spdlog::level::warn: {

                    if (!showWarn) continue;
                    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.9f, 0.4f, 1.0f));

                    break;

                }
                case spdlog::level::err: {

                    if (!showError) continue;
                    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.2f, 0.2f, 1.0f));

                    break;

                }
                default: continue;
                
            }

            ImGui::TextWrapped("%s", msg.text.c_str());
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
