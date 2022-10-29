#pragma once

#include "Engine/Core/Log.h"

#include "Engine/Utilities/Wrappers.h"

namespace Copper {

    template<typename T> using Unique = std::unique_ptr<T>;
    template<typename T, typename ... Args> constexpr  Unique<T> CreateUnique(Args&& ... args) {

        return std::make_unique<T>(std::forward<Args>(args)...);
        
    }

    template<typename T> using Shared = std::shared_ptr<T>;
    template<typename T, typename ... Args> constexpr  Shared<T> CreateShared(Args&& ... args) {

        return std::make_shared<T>(std::forward<Args>(args)...);
        
    }
    
}