#pragma once

#include "Engine/Core/Core.h"
#include "Engine/Core/Window.h"

class ImGuiContext;

namespace Copper {

	class UI {

	public:
		void Initialize(const Window& window, bool gizmo = false, bool docking = true, bool viewports = true);
		void Shutdown();

		void Begin();
		void End();

		void LoadFont(const std::string& path, float fontSize = 18.0f);

		void SetAsCurrent();
	
	private:
		bool gizmo;
		bool docking;
		bool viewports;

		ImGuiContext* context;

	};

}