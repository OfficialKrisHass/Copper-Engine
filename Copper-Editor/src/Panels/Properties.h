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

		bool ShowVector2(std::string name, Copper::Vector2& vec, float speed = 0.01f);
		bool ShowVector3(std::string name, Copper::Vector3& vec, float speed = 0.01f);
		bool ShowVector4(std::string name, Copper::Vector4& vec, float speed = 0.01f);
		bool ShowColor(std::string name, Copper::Color& col, float speed = 0.01f);
		bool ShowInt(std::string name, int& show, float speed = 1);
		bool ShowUInt(std::string name, unsigned int& show, float speed = 1.0f);
		bool ShowFloat(std::string name, float& show, float speed = 0.01f);
		bool ShowDouble(std::string name, double& show, float speed = 0.01f);
		bool ShowString(std::string name, std::string& show);
		bool ShowChar(std::string name, char& show);

	private:
		Copper::Object selectedObj;
		static std::filesystem::path selectedFile;
		static bool wasFileLast;

		virtual void UI() override;

		void RenderObject();
		void RenderFile();

		template<typename T, typename F> void ShowScriptField(Copper::ScriptComponent* script, const Copper::ScriptField& field, F showFunc);

	};

}