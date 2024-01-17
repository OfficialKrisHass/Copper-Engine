list(APPEND windowsSources WindowsInput.cpp WindowsWindow.cpp)
list(TRANSFORM windowsSources PREPEND src/Platform/Windows/)

list(APPEND sources ${windowsSources})