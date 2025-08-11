list(APPEND coreSources EditorApp.h SceneSerializer.h FieldSerializer.h
                        EditorApp.cpp SceneSerializer.cpp FieldSerializer.cpp Change.cpp ChangeHandler.cpp Clipboard.cpp)
list(TRANSFORM coreSources PREPEND src/Core/)

list(APPEND sources ${coreSources})
