#include "LauncherApp.h"

#include "UI.h"
#include "Window.h"
#include "Project.h"

#include "OS/WindowsHelpers.h"

#include <ImGui/imgui.h>
#include <yaml-cpp/yaml.h>

#include <corecrt_wstring.h>

#include <iostream>
#include <fstream>
#include <vector>

#define WindowFlags ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove

namespace Launcher {

	struct Data {

		bool running = true;

		std::vector<Project> projects;

	};
	Data data;

	//De/Serialization
	void SerializeProjects();
	void DeserializeProjects();

	//Projects
	void NewProject();

	//UI
	void StartDockspace();

	//Events
	void OnWindowClose();

	void Initialize() {

		Window::Initialize();
		UI::Initialize();

		UI::LoadFont("assets/Fonts/open-sans.regular.ttf");

		DeserializeProjects();

	}
	void Run() {

		while (data.running) {

			Window::Update();
			UI::Begin();
			StartDockspace();

			ImGui::Begin("Main Window", nullptr); // Main Window

			ImVec2 buttonSize = ImVec2(175, 50);
			if (ImGui::Button("New Project", buttonSize)) {

				NewProject();

			}

			ImGui::End(); // Main Window



			ImGui::Begin("Project List"); //Project List

			buttonSize = ImVec2(ImGui::GetContentRegionAvailWidth(), 150);

			int index = 0;
			for (Project& project : data.projects) {

				ImGui::PushID(index);

				ImGui::GetFont()->FontSize -= 7.0f;
				ImGui::PushStyleVar(ImGuiStyleVar_ButtonTextAlign, ImVec2(0.0f, 0.0f));

				std::string buttonText = project.name + "\n\n        " + project.path.string();
				if (ImGui::Button(buttonText.c_str(), buttonSize)) {

					std::string path = project.path.string();
					size_t spaceIndex = path.find(' ');
					while (spaceIndex != std::string::npos) {

						path.replace(spaceIndex, 1, "\" \"");
						spaceIndex = path.find(' ', spaceIndex + 2);

					}
					std::wstring wPath(path.begin(), path.end());

					//Open the Editor
					OS::StartApplication("C:\\Programming\\Copper-Engine\\Build\\windows-x86_64-Debug\\Copper-Editor\\Copper-Editor.exe", wPath);
					data.running = false;
				
				}
				ImGui::PopStyleVar();
				ImGui::GetFont()->FontSize += 7.0f;

				ImGui::PopID();
				index++;

			}

			ImGui::End(); // Project List

			ImGui::End(); // Dockspace
			UI::End();

		}

	}
	void Shutdown() {

		SerializeProjects();

		UI::Shutdown();
		Window::Shutdown();

	}

	void SerializeProjects() {

		YAML::Emitter out;

		out << YAML::BeginMap; // Main

		out << YAML::Key << "Projects" << YAML::Value << YAML::BeginMap; // Projects
		for (const Project& project : data.projects) {

			out << YAML::Key << project.name << YAML::Value << YAML::BeginMap; // Project

			out << YAML::Key << "Path" << YAML::Value << project.path.string();

			out << YAML::EndMap; // Project

		}
		out << YAML::EndMap; // Projects

		out << YAML::EndMap; // Main
		std::ofstream file("assets/LauncherData.cu");
		file << out.c_str();

	}
	void DeserializeProjects() {

		YAML::Node launcherData;
		try { launcherData = YAML::LoadFile("assets/LauncherData.cu"); }
		catch (YAML::ParserException e) { std::cout << "Failed to Load Launcher Data file!\n\n" << e.what() << std::endl; }

		YAML::Node projects = launcherData["Projects"];
		for (YAML::const_iterator it = projects.begin(); it != projects.end(); ++it) {

			YAML::Node project = it->second;

			std::string name = it->first.as<std::string>();
			std::filesystem::path path = project["Path"].as<std::string>();

			data.projects.push_back(Project(name, path));

		}

	}

	void NewProject() {

		std::string path = "C:\\Programming\\Copper-Engine\\Editor Projects\\TestingProject";
		/*std::string path = OS::OpenFolderDialog().string();
		if (path.empty()) { std::cerr << "Path is Invalid or Empty!" << std::endl; return; }*/

		size_t parentEnd = path.find_last_of("\\") + 1;
		std::string name = path.substr(parentEnd, path.size() - parentEnd);

		data.projects.push_back(Project(name, path));
		Project& project = data.projects.back();

		//Create the Directioes
		std::filesystem::create_directories(path + "\\Assets\\Scenes");
		std::filesystem::create_directories(path + "\\Binaries");
		std::filesystem::create_directories(path + "\\Objs");

		//Copy the Template scene and it's cum (winky face)
		std::ifstream templateScene("assets\\Projects\\EmptyTemplate\\Assets\\Scenes\\EmptyTemplate.copper");
		std::fstream projectScene;

		projectScene.open(path + "\\Assets\\Scenes\\EmptyTemplate.copper", std::ios::out);
		projectScene << templateScene.rdbuf();
		projectScene.close();

		std::ifstream templateSceneCUM("assets\\Projects\\EmptyTemplate\\Assets\\Scenes\\EmptyTemplate.copper.cum");
		std::fstream projectSceneCUM;

		projectSceneCUM.open(path + "\\Assets\\Scenes\\EmptyTemplate.copper.cum", std::ios::out);
		projectSceneCUM << templateSceneCUM.rdbuf();
		projectSceneCUM.close();

		//Copy the Project.cu file
		std::ifstream templateProjectFile("assets\\Projects\\EmptyTemplate\\Project.cu");
		std::fstream projectFile;

		projectFile.open(path + "\\Project.cu", std::ios::out);
		projectFile << templateProjectFile.rdbuf();
		projectFile.close();

		//Copy the ScriptingAPI dll
		std::ifstream dllSrc("assets\\ScriptAPI\\Copper-ScriptingAPI.dll", std::ios::binary);
		std::fstream dllDst;

		dllDst.open(path + "\\Binaries\\Copper-ScriptingAPI.dll", std::ios::out | std::ios::binary);
		dllDst << dllSrc.rdbuf();
		dllDst.close();

		//Create, the Solution and Assembly
		std::ifstream templateSln("assets\\Projects\\EmptyTemplate\\Template.sln.cut");
		std::fstream projectSln;
		projectSln.open(path + "\\" + name + ".sln", std::ios::out);

		//Put in the Correct stuff into the solution file, idk what I'm doing help
		std::string line;
		while (std::getline(templateSln, line)) {

			size_t pos = line.find(':');
			while (pos != std::string::npos && line[pos + 1] == '{') {

				size_t end = line.find_first_of('}', pos);
				std::string var = line.substr(pos + 2, end - (pos + 2));

				if (var == "ProjectName") {

					line.erase(pos, (end - pos) + 1);
					line.insert(pos, name);

				}

				pos = line.find(':', pos + 1);

			}

			projectSln << line << "\n";

		}
		projectSln.close();

		//Put the correct things into the .csproj file. I Think
		std::ifstream templateCsproj("assets\\Projects\\EmptyTemplate\\Template.csproj.cut");
		std::fstream projectCsproj;
		projectCsproj.open(path + "\\" + name + ".csproj", std::ios::out);

		line = "";
		while (std::getline(templateCsproj, line)) {

			size_t pos = line.find(':');
			while (pos != std::string::npos && line[pos + 1] == '{') {

				size_t end = line.find_first_of('}', pos);
				std::string var = line.substr(pos + 2, end - (pos + 2));

				if (var == "ProjectName") {

					line.erase(pos, (end - pos) + 1);
					line.insert(pos, name);

				}

				pos = line.find(':', pos + 1);

			}

			projectCsproj << line << "\n";

		}
		projectCsproj.close();
		
		//Build the Solution
		std::string cmd = "C:\\Windows\\Microsoft.NET\\Framework\\v4.0.30319\\MSBuild.exe ";

		size_t pos = path.find_first_of(' ');
		std::string newPath = path;
		while (pos != std::string::npos) {

			newPath.erase(pos, 1);
			newPath.insert(pos, "\" \"");
			pos = newPath.find_first_of(' ', pos + 3);

		}

		pos = name.find_first_of(' ');
		std::string newName = name;
		while (pos != std::string::npos) {

			newName.erase(pos, 1);
			newName.insert(pos, "\" \"");
			pos = newName.find_first_of(' ', pos + 3);

		}

		cmd += newPath + "\\" + newName + ".csproj";
		cmd += " -nologo";

		system(cmd.c_str());

		size_t spaceIndex = path.find(' ');
		while (spaceIndex != std::string::npos) {

			path.replace(spaceIndex, 1, "\" \"");
			spaceIndex = path.find(' ', spaceIndex + 2);

		}

		std::wstring wPath(path.begin(), path.end());

		//Open the Editor
		OS::StartApplication("C:\\Programming\\Copper-Engine\\Build\\windows-x86_64-Debug\\Copper-Editor\\Copper-Editor.exe", wPath);
		data.running = false;

	}

	void StartDockspace() {

		static bool dockspaceOpen = true;
		static bool opt_fullscreen_persistant = true;
		bool opt_fullscreen = opt_fullscreen_persistant;
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

		ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking;

		if (opt_fullscreen) {

			ImGuiViewport* viewport = ImGui::GetMainViewport();

			ImGui::SetNextWindowPos(viewport->Pos);
			ImGui::SetNextWindowSize(viewport->Size);
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

			window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

		}

		if (dockspace_flags & ImGuiDockNodeFlags_PassthruDockspace) window_flags |= ImGuiWindowFlags_NoBackground;

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("DockSpace Demo", &dockspaceOpen, window_flags); //Dockspace
		ImGui::PopStyleVar();

		if (opt_fullscreen) ImGui::PopStyleVar(2);

		ImGuiIO& io = ImGui::GetIO();
		ImGuiStyle& style = ImGui::GetStyle();

		float minWinSizeX = style.WindowMinSize.x;
		style.WindowMinSize.x = 370.0f;

		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable) {

			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");

			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);

		}

		style.WindowMinSize.x = minWinSizeX;

	}

	void OnWindowClose() { data.running = false; }

}

int main() {

	Launcher::Initialize();
	Launcher::Run();
	Launcher::Shutdown();

	return 0;

}