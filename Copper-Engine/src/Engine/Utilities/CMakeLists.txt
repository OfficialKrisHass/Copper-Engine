list(APPEND utilitiesSources FileUtils.h Math.h
                             FileUtils.cpp Math.cpp)
list(TRANSFORM utilitiesSources PREPEND src/Engine/Utilities/)

list(APPEND sources ${utilitiesSources})