list(APPEND coreSources EditorApp.h FileWatcher.h SceneSerializer.h
                        EditorApp.cpp FileWatcher.cpp SceneSerializer.cpp Change.cpp ChangeHandler.cpp Clipboard.cpp)
list(TRANSFORM coreSources PREPEND src/Core/)

list(APPEND sources ${coreSources})
