#pragma once

#include "Engine/Core/Core.h"
#include "Engine/Core/Window.h"

struct ImGuiContext;

namespace Copper {

	class UIContext {

	public:
		UIContext() = default;

		void Initialize(const Window& window, bool gizmo = false, bool docking = true, bool viewports = true);
		void Shutdown();

		void Begin();
		void End();

		void LoadFont(const std::string& path, float fontSize = 18.0f);

		void SetAsCurrent();
	
	private:
		bool gizmo = false;
		bool docking = true;
		bool viewports = true;

		ImGuiContext* context = nullptr;

	};

	void SetMainUIAsCurrent();
	void LoadMainUIContextFont(const std::string& path, float fontSize = 18.0f);

}