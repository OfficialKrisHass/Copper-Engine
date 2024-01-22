#pragma once

#include "Engine/Core/Core.h"

namespace Copper {

    class Args {

    public:
        Args(uint32 argc, char* argv[]) {
            
            m_args.resize(argc, nullptr);
            for (uint32 i = 1; i < argc; i++) {
            
#ifdef CU_LINUX
                if (std::string(argv[i]) == "./Copper-Editor") continue;
#endif
                m_args[i] = argv[i];

            }

        }

        inline uint32 Count() const { return (uint32) m_args.size(); }
        inline char* GetArg(const uint32 index) const { return m_args[index]; }

    private:
        std::vector<char*> m_args;

    };

}