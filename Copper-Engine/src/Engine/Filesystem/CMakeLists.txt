list(APPEND filesystemSources Iterator.h Path.h Path.inl
                              Path.cpp)
list(TRANSFORM filesystemSources PREPEND src/Engine/Filesystem/)

list(APPEND sources ${filesystemSources})