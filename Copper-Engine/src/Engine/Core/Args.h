#pragma once

#include "Engine/Core/Core.h"

namespace Copper {

    class Args {

    public:
        Args(int argc, char* argv[]) {
            
            args.resize(argc, nullptr);
            for (int i = 1; i < argc; i++) {
            
#ifdef CU_LINUX
                if (std::string(argv[i]) == "./Copper-Editor") continue;
#endif
                args[i] = argv[i];

            }

        }

        inline uint32_t Count() const { return (uint32_t) args.size(); }
        inline char* GetArg(const uint32_t index) const { return args[index]; }

    private:
        std::vector<char*> args;

    };

}