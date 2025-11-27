#include "ProjectBuilder.h"

#include <regex>

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

    bool BuildScripts(const fs::path &path) {

        CUP_FUNCTION();

#ifdef CU_WINDOWS
        std::string cmd = "C:\\Windows\\Microsoft.NET\\Framework\\v4.0.30319\\MSBuild.exe ";

        size_t pos = path.string().find_first_of(' ');
        std::string newPath = path.string();
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
#else
        const std::string cmd = "make --no-print-directory -C \"" + path.string() + "\" -f Makefile 2>&1";

        FILE* make = popen(cmd.c_str(), "r");
        CU_ASSERT(make != nullptr, "Could not open pipe to run makefile. Command: '{}'", cmd);

        // Assets/PlayerMovement.cs(11,20): warning CS0649: Field 'PlayerMovement.test' is never assigned to, and will always have its default value null
        //
        // Group 0 - whole line
        // Group 1 - File path (Assets/PlayerMovement.cs)
        // Group 2 - Line number (11)
        // Group 3 - Optional column number (20)
        // Group 4 - either the word warning or error (warning)
        // Group 5 - diagnostic code (CS0649)
        // Group 6 - message (Field 'PlayerMovement.test'...)
        //
        // I hate regex. This took me an hour to figure out :)
        const std::regex regex(R"(^(.+?)\((\d+)(?:,(\d+))?\):\s*(warning|error)\s+(CS\d+):\s*(.+)\s)", std::regex::ECMAScript | std::regex::icase);

        // This seems too large, but in very rare cases, an error or warning might be incredibly long.
        // Thankfully this code is not ran that often so it's a worthy sacrifice.
        char buffer[4096];
        while (fgets(buffer, sizeof(buffer), make) != nullptr) {

            const std::string line = buffer;
            std::smatch match;

            if (!std::regex_search(line, match, regex)) continue;

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

        pclose(make);
#endif

        return true;

    }

}
