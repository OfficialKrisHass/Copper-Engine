list(APPEND filesystemSources FileWatch.cpp DirWatch.cpp RecursiveDirWatch.cpp)
list(TRANSFORM filesystemSources PREPEND src/Engine/Filesystem/)

list(APPEND sources ${filesystemSources})
