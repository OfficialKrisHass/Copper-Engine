list(APPEND linuxSources LinuxFileUtils.cpp LinuxInput.cpp LinuxPopup.cpp LinuxWindow.cpp)
list(TRANSFORM linuxSources PREPEND src/Platform/Linux/)

list(APPEND sources ${linuxSources})