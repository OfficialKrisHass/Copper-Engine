list(APPEND projectsSources Project.h ProjectTemplate.h
                            Project.cpp ProjectTemplate.cpp)
list(TRANSFORM projectsSources PREPEND src/Projects/)

list(APPEND sources ${projectsSources})