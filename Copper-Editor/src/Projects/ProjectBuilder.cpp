#include "ProjectBuilder.h"

#include <regex>

#ifdef CU_WINDOWS
#include <windows.h>
#endif

namespace Editor::ProjectBuilder {

    struct BuildMessage {

        enum class Severity : uint8 {

            Warning = 0,
            Error,

            None,

        };

        fs::path file;

        uint32 line = 0;
        uint32 column = 0;

        Severity severity = Severity::None;

        std::string code;
        std::string message;

    };

    void LineParser(const std::string& line);

#ifdef CU_WINDOWS
    fs::path GetMSBuildPath();
#endif

    bool BuildScripts(const fs::path& path) {

        CUP_FUNCTION();

        // This seems too large, but in very rare cases, an error or warning might be incredibly long.
        // Thankfully this code is not ran that often so it's a worthy sacrifice.
        static char buffer[4096];

#ifdef CU_WINDOWS
        // I ABSOLUTELY HATE THE WINDOWS API
        // genuinely look how simple the linux api is. It's literally 13 fucking lines, THIRTEEN
        // THE WINDOWS VERSION IS LIKE 50 SKHKJSHDJHSHGDHGHSJDGHJD

        static const fs::path msbuild = GetMSBuildPath();

        // Setup and create the pipe

        SECURITY_ATTRIBUTES sa;
        ZeroMemory(&sa, sizeof(sa));
        sa.nLength = sizeof(sa);
        sa.bInheritHandle = true;

        HANDLE read, write;
        bool success = CreatePipe(&read, &write, &sa, 0);
        CU_ASSERT(success, "Failed to create pipe for MSBuild.exe");

        SetHandleInformation(read, HANDLE_FLAG_INHERIT, 0);

        // Actually run MSBuild

        const std::string solutionPath = (path / path.filename().replace_extension(".csproj")).string();
        std::string cmd = "\"" + msbuild.string() + "\" \"" + solutionPath + "\" /nologo /verbosity:minimal";

        PROCESS_INFORMATION processInfo = {};
        STARTUPINFOA startupInfo = {};

        startupInfo.cb = sizeof(startupInfo);
        startupInfo.hStdOutput = write;
        startupInfo.hStdError = write;
        startupInfo.dwFlags = STARTF_USESTDHANDLES;

        if (!CreateProcessA(nullptr, cmd.data(), nullptr, nullptr, true, CREATE_NO_WINDOW, nullptr, nullptr, &startupInfo, &processInfo)) {

            LogError("Could not run MSBUild.exe. cmd: {}", cmd);

            CloseHandle(read);
            CloseHandle(write);

            return false;

        }

        CloseHandle(write);

        // Read stdout/stderr

        DWORD bytesRead = 0;
        std::string line;

        while (true) {

            if (!ReadFile(read, buffer, sizeof(buffer), &bytesRead, nullptr) || bytesRead == 0) break;

            for (uint32 i = 0; i < bytesRead; i++) {

                if (buffer[i] == '\n') {

                    LineParser(line);
                    line.clear();

                    continue;

                }
                if (buffer[i] == '\r') continue;

                line += buffer[i];

            }

        }

        if (!line.empty())
            LineParser(line);

        // Cleanup

        CloseHandle(read);

        WaitForSingleObject(processInfo.hProcess, INFINITE);
        CloseHandle(processInfo.hThread);
        CloseHandle(processInfo.hProcess);
#else
        const std::string cmd = "make --no-print-directory -C \"" + path.string() + "\" -f Makefile 2>&1";

        FILE* make = popen(cmd.c_str(), "r");
        CU_ASSERT(make != nullptr, "Could not open pipe to run makefile. Command: '{}'", cmd);

        while (fgets(buffer, sizeof(buffer), make) != nullptr) {

            const std::string line = buffer;
            LineParser(line);

        }

        pclose(make);
#endif

        return true;

    }

    void LineParser(const std::string& line) {

        CUP_FUNCTION();

        // C:\Programming\Copper-Engine\Dev Projects\GuideProject\Assets\PlayerMovement.cs(20,33): error CS1061: 'Vector3' does not contain a definition for 'Normaize' and no accessible extension method 'Normaize' accepting a first argument of type 'Vector3' could be found (are you missing a using directive or an assembly reference?) [C:\Programming\Copper-Engine\Dev Projects\GuideProject\GuideProject.csproj]
        //
        // Group 0 - Whole line
        // Group 1 - File path (Assets/PlayerMovement.cs)
        // Group 2 - Line number (20)
        // Group 3 - Optional column number (33)
        // Group 4 - The word error or warning (error)
        // Group 5 - Diagnostic code (CS1061)
        // Group 6 - Actual build message ('Vector3' does not contain...)

#ifdef CU_WINDOWS
        static const std::regex regex = std::regex(R"(^.+(Assets\\.+)\((\d+)(?:,(\d+?))?\):\s*(warning|error)\s*(CS\d+):\s*(.+)\s+\[.+\]$)", std::regex::ECMAScript | std::regex::icase);
#elif CU_LINUX
        static const std::regex regex(R"(^(.+?)\((\d+)(?:,(\d+))?\):\s*(warning|error)\s+(CS\d+):\s*(.+)\s)", std::regex::ECMAScript | std::regex::icase);
#endif

        std::smatch match;

        if (!std::regex_search(line, match, regex)) return;

        BuildMessage msg;

        msg.file = match[1].str();

        msg.line = std::stoi(match[2].str());
        if (match[3].matched) msg.column = std::stoi(match[3].str());

        if (match[4].str() == "warning")
            msg.severity = BuildMessage::Severity::Warning;
        else if (match[4].str() == "error")
            msg.severity = BuildMessage::Severity::Error;
        else {

            LogError("Invalid build message severity! {}", line);

            msg.severity = BuildMessage::Severity::None;

        }

        msg.code = match[5].str();
        msg.message = match[6].str();

        if (msg.severity == BuildMessage::Severity::Warning)
            LogWarn("{}:({},{}): {} ({})", msg.file, msg.line, msg.column, msg.message, msg.code);
        else if (msg.severity == BuildMessage::Severity::Error)
            LogError("{}:({},{}): {} ({})", msg.file, msg.line, msg.column, msg.message, msg.code);

    }

#ifdef CU_WINDOWS
    fs::path GetMSBuildPath() {

        CUP_FUNCTION();

        static const char* cmd = "\"C:\\Program Files (x86)\\Microsoft Visual Studio\\Installer\\vswhere.exe\" "
            "-latest -requires Microsoft.Component.MSBuild -find MSBuild\\**\\Bin\\MSBuild.exe";

        FILE* pipe = _popen(cmd, "r");
        if (pipe == nullptr) {

            LogError("Could not open pipe to vswhere. Command: {}", cmd);
            return "";

        }

        static char buffer[4096];
        std::string ret;

        while (fgets(buffer, sizeof(buffer), pipe))
            ret += buffer;

        _pclose(pipe);

        ret.erase(ret.find_last_not_of(" \r\n\t") + 1);
        return ret;

    }
#endif

}
