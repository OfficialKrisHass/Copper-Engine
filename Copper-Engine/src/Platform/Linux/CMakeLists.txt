list(APPEND linuxSources LinuxInput.cpp LinuxWindow.cpp LinuxFileWatch.cpp LinuxDirWatch.cpp LinuxRecursiveDirWatch.cpp)
list(TRANSFORM linuxSources PREPEND src/Platform/Linux/)

list(APPEND sources ${linuxSources})
