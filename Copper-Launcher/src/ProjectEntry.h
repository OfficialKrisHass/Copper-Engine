#pragma once

#include "Base.h"

namespace Launcher {

	class ProjectEntry {

	public:
		ProjectEntry(const std::string& name, const std::string& directory) : m_name(name), m_directory(directory) {}

		static void InitializeFonts();

		void Render() const;

	private:
		std::string m_name = "";
		std::string m_directory = "";

	};

}