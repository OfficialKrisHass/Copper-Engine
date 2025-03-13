list(APPEND coreSources EditorApp.h FileWatcher.h SceneSerializer.h
                        EditorApp.cpp FileWatcher.cpp SceneSerializer.cpp
                        Utils/ModelLoader.h Utils/ModelLoader.cpp)
list(TRANSFORM coreSources PREPEND src/Core/)

list(APPEND sources ${coreSources})
