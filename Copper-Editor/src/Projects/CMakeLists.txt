list(APPEND projectsSources Project.h ProjectTemplate.h ProjectChecker.h ProjectMetadata.h ProjectBuilder.h
                            Project.cpp ProjectTemplate.cpp ProjectChecker.cpp ProjectMetadata.cpp ProjectBuilder.cpp)
list(TRANSFORM projectsSources PREPEND src/Projects/)

list(APPEND sources ${projectsSources})
