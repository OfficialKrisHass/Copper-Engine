#include "Console.h"

using namespace Copper;

namespace Editor {

	void Console::UI() {

		/*if (ImGui::Button("Test")) Log("Test");

		ImGui::GetFont()->FontSize -= 2.0f;

		for (int i = (int) Logger::GetLastLogMessages().size() - 1; i >= 0; i--) {

			std::string msg = Logger::GetLastLogMessages()[i];

			size_t endOfLevel = msg.find_first_of(':');
			std::string level = msg.substr(0, endOfLevel);
			msg.erase(0, endOfLevel + 1);

			if (level == "trace") ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
			else if (level == "warning") ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.9f, 0.4f, 1.0f));
			else if (level == "error") ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.2f, 0.2f, 1.0f));

			ImGui::Text(msg.c_str());
			ImGui::Separator();

			ImGui::PopStyleColor();

		}

		ImGui::GetFont()->FontSize += 2.0f;*/

	}

}