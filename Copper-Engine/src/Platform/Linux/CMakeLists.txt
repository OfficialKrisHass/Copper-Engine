list(APPEND linuxSources LinuxInput.cpp LinuxWindow.cpp)
list(TRANSFORM linuxSources PREPEND src/Platform/Linux/)

list(APPEND sources ${linuxSources})