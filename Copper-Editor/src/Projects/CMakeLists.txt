list(APPEND projectsSources Project.h ProjectTemplate.h ProjectChecker.h ProjectMetadata.h
                            Project.cpp ProjectTemplate.cpp ProjectChecker.cpp ProjectMetadata.cpp)
list(TRANSFORM projectsSources PREPEND src/Projects/)

list(APPEND sources ${projectsSources})
