#include "OutFile.h"

#include <iostream>

namespace APIBinder {

    std::string Tab(uint32 n) {

        std::string ret = "";
        for (uint32 i = 0; i < n * 4; i++)
            ret += ' ';

        return ret;

    }

    void BeginOutFile(std::ofstream& stream) {

        // Open

        stream.open("Copper-Engine/src/Engine/Scripting/InternalCalls.cpp");
        if (!stream.is_open()) {

            std::cerr << "Could not open Output file Copper-Engine/src/Engine/Scripting/InternalCalls.cpp\n";
            exit(1);

        }
        stream.clear();

        // Write 

        stream << "#include \"cupch.h\"\n\n";

    }
    void MiddleOutFile(std::ofstream& stream) {

        stream << "\n#include <mono/jit/jit.h>\n\n";
        stream << "namespace Copper::Scripting {\n\n";

        stream << Tab(1) << "void SetupInternalCalls() {\n\n";
        stream << Tab(2) << "CUP_FUNCTION();\n\n";
        stream << Tab(2) << "Entity::Initialize();\n\n";

    }
    void FinishOutFile(std::ofstream& stream) {

        stream << "\n";
        stream << Tab(1) << "}\n\n}";
        stream.close();

    }

}
