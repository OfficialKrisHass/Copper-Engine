list(APPEND windowsSources WindowsError.cpp WindowsFileUtils.cpp WindowsInput.cpp WindowsWindow.cpp)
list(TRANSFORM windowsSources PREPEND src/Platform/Windows/)

list(APPEND sources ${windowsSources})