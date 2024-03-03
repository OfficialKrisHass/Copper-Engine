list(APPEND utilitiesSources FileUtils.h Math.h FileTemplate.h
                             FileUtils.cpp Math.cpp FileTemplate.cpp)
list(TRANSFORM utilitiesSources PREPEND src/Engine/Utilities/)

list(APPEND sources ${utilitiesSources})