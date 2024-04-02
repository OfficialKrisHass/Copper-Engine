#include "ProjectEntry.h"

#include "UI.h"
#include "PersistentData.h"

#include "Utils.h"

#define IMGUI_DEFINE_MATH_OPERATORS
#include <ImGui/imgui.h>
#include <ImGui/imgui_internal.h>

#ifdef CU_LINUX
#include <sys/types.h>
#include <unistd.h>
#elif CU_WINDOWS
#include <Windows.h>
#include <processthreadsapi.h>
#endif

namespace Launcher {

	constexpr float ProjectTabHeight = 85.0f;

	static ImFont* titleFont = nullptr;
	static ImFont* detailsFont = nullptr;

	static ImGuiID heldID = ImGuiID(0);

  extern void OnWindowClose();

	void ProjectEntry::InitializeFonts() {

		titleFont = UI::AddFont(ExecutableFolder() + MainFontPath, 35.0f);
		detailsFont = UI::AddFont(ExecutableFolder() + MainFontPath, 18.0f);

	}

	void ProjectEntry::Render() const {

		const ImGuiID id = ImGui::GetID(m_name.c_str());
		const ImVec2 size = { ImGui::GetContentRegionAvail().x, ProjectTabHeight };
		const bool isHovered = heldID == ImGuiID(0) && ImGui::GetHoveredID() == id;
		const bool isHeld = heldID == id;

		if (isHeld)
			ImGui::PushStyleColor(ImGuiCol_FrameBg, ImGui::GetStyleColorVec4(ImGuiCol_ButtonActive));
		else if (isHovered)
			ImGui::PushStyleColor(ImGuiCol_FrameBg, ImGui::GetStyleColorVec4(ImGuiCol_ButtonHovered));

		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, { WindowPadding, 0 });
		ImGui::BeginChildFrame(id, size, ImGuiWindowFlags_NoScrollbar);
		ImGui::PopStyleVar();

		if (isHovered || isHeld)
			ImGui::PopStyleColor();

		const ImRect tabRect = { ImGui::GetItemRectMin(), ImGui::GetItemRectMin() + size };

		ImGui::PushFont(titleFont);
		ImGui::Text(m_name.c_str());
		ImGui::PopFont();

		ImGui::SetCursorPosY(ImGui::GetContentRegionMax().y - ImGui::GetTextLineHeight());

		ImGui::PushFont(detailsFont);
		ImGui::Text(m_directory.c_str());
		ImGui::PopFont();

		bool held = false;
		if (ImGui::ButtonBehavior(tabRect, id, nullptr, &held, ImGuiButtonFlags_PressedOnClickRelease | ImGuiButtonFlags_PressedOnRelease)) {

#ifdef CU_LINUX
      pid_t pid = fork();

      if (pid == 0) { // Child procces

        char* args[] = { (char*) PersistentData::EditorPath().data(), (char*) m_directory.data(), nullptr };
        execv(PersistentData::EditorPath().c_str(), args);

      } else
        OnWindowClose();
#elif CU_WINDOWS
			std::string editorPath = Utils::ReplaceSpaces(PersistentData::EditorPath());
			std::string projectPath = Utils::ReplaceSpaces(m_directory);
			std::string args = editorPath + " " + projectPath;

			STARTUPINFOA si;
			PROCESS_INFORMATION pi;
			
			ZeroMemory(&si, sizeof(si));
			ZeroMemory(&pi, sizeof(pi));
			si.cb = sizeof(si);

			CreateProcessA(editorPath.c_str(), args.data(), NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);
			CloseHandle(pi.hProcess);
			CloseHandle(pi.hThread);

			OnWindowClose();
#endif

    }

		if (held)
			heldID = id;
		else if (heldID == id && ImGui::IsMouseReleased(ImGuiMouseButton_Left))
			heldID = ImGuiID(0);

		ImGui::EndChildFrame();

	}

}
