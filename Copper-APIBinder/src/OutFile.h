#pragma once

#include "Base.h"

#include <fstream>

namespace APIBinder {

    std::string Tab(uint32 n);

    void BeginOutFile(std::ofstream& stream);
    void MiddleOutFile(std::ofstream& stream);
    void FinishOutFile(std::ofstream& stream);

}
