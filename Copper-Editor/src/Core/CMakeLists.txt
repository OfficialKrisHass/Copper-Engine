list(APPEND coreSources EditorApp.h SceneSerializer.h
                        EditorApp.cpp SceneSerializer.cpp Change.cpp ChangeHandler.cpp Clipboard.cpp)
list(TRANSFORM coreSources PREPEND src/Core/)

list(APPEND sources ${coreSources})
