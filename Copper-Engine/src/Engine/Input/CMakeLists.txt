list(APPEND inputSources AxisManager.h Input.h KeyCodes.h Popup.h
                         AxisManager.cpp)
list(TRANSFORM inputSources PREPEND src/Engine/Input/)

list(APPEND sources ${inputSources})