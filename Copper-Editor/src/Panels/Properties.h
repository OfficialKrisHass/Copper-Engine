#pragma once

#include "Copper.h"

#include "Panels/Panel.h"

#include <filesystem>

namespace Editor {

	class Properties : public Panel {

	public:
		Properties() : Panel("Properties") {}

		void SetSelectedObject(Copper::Object obj) {
			if(selectedObj == obj) return;
			
			this->selectedObj = obj;
			if(obj) { wasFileLast = false; }
			
		}
		static void SetSelectedFile(std::filesystem::path file) { selectedFile = file;  wasFileLast = true; }

	private:
		Copper::Object selectedObj;
		static std::filesystem::path selectedFile;
		static bool wasFileLast;

		virtual void UI() override;

		void RenderObject();
		void RenderFile();

		bool ShowVector2(std::string name, Copper::Vector2& vec, float speed = 0.01f);
		bool ShowVector3(std::string name, Copper::Vector3& vec, float speed = 0.01f);
		bool ShowVector4(std::string name, Copper::Vector4& vec, float speed = 0.01f);
		bool ShowColor(std::string name, Copper::Color& col, bool showAlpha = true);
		bool ShowInt(std::string name, int& show, int speed = 1);
		bool ShowUInt(std::string name, unsigned int& show, int speed = 1);
		bool ShowFloat(std::string name, float& show, float speed = 0.01f);
		bool ShowDouble(std::string name, double& show, float speed = 0.01f);
		bool ShowString(std::string name, std::string& show);
		bool ShowChar(std::string name, char& show);

	};

}