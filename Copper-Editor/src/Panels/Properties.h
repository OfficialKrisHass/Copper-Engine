#pragma once

#include "Copper.h"

#include "Panels/Panel.h"

namespace Editor {

	class Properties : public Panel {

	public:
		Properties() : Panel("Properties") {}

		inline void SetSelectedObject(Copper::Object obj) { this->selectedObj = obj; }

	private:
		Copper::Object selectedObj;

		virtual void UI() override;

		void ShowVector3(std::string name, Copper::Vector3& vec, float speed = 0.01f);
		void ShowFloat(std::string name, float& show, float speed = 0.01f);

	};

}