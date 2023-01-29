#pragma once

#include "Copper.h"

#include "Panels/Panel.h"

#include <filesystem>

namespace Editor {

	class Properties : public Panel {

	public:
		Properties();

		void SetSelectedObject(Copper::Object obj) {
			if(selectedObj == obj) return;
			
			this->selectedObj = obj;
			if(obj) { wasFileLast = false; }
			
		}
		static void SetSelectedFile(std::filesystem::path file) { selectedFile = file;  wasFileLast = true; }

		bool ShowInt(const std::string& name, int& show, float speed = 1);
		bool ShowUInt(const std::string& name, unsigned int& show, float speed = 1.0f);
		bool ShowFloat(const std::string& name, float& show, float speed = 0.01f);
		bool ShowDouble(const std::string& name, double& show, float speed = 0.01f);
		bool ShowString(const std::string& name, std::string& show);
		bool ShowChar(const std::string& name, char& show);

		bool ShowVector2(const std::string& name, Copper::Vector2& vec, float speed = 0.01f, bool verticalSpacing = true);
		bool ShowVector3(const std::string& name, Copper::Vector3& vec, float speed = 0.01f, bool verticalSpacing = true);
		bool ShowVector4(const std::string& name, Copper::Vector4& vec, float speed = 0.01f, bool verticalSpacing = true);
		bool ShowColor(const std::string& name, Copper::Color& col, float speed = 0.01f);

		bool ShowObject(const std::string& name, Copper::Object& obj);
		bool ShowComponent(const std::string& name, Copper::Component* component);

		static bool IsDragDropTargetHovered() { return dragDropTargetHovered; }
	
	private:
		Copper::Object selectedObj;
		static std::filesystem::path selectedFile;
		static bool wasFileLast;

		static bool dragDropTargetHovered;

		virtual void UI() override;

		void RenderObject();
		void RenderFile();

		template<typename T, typename F> void RenderScriptField(Copper::ScriptComponent* script, const Copper::ScriptField& field, F showFunc);

	};

}