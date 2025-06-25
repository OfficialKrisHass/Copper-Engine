list(APPEND filesystemSources FileWatch.cpp)
list(TRANSFORM filesystemSources PREPEND src/Engine/Filesystem/)

list(APPEND sources ${filesystemSources})
