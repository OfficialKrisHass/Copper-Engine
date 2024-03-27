list(APPEND projectsSources Project.h ProjectTemplate.h ProjectChecker.h
                            Project.cpp ProjectTemplate.cpp ProjectChecker.cpp)
list(TRANSFORM projectsSources PREPEND src/Projects/)

list(APPEND sources ${projectsSources})
