list(APPEND coreSources EditorApp.h FileWatcher.h SceneMeta.h SceneSerializer.h
                        EditorApp.cpp FileWatcher.cpp SceneMeta.cpp SceneSerializer.cpp
                        Utils/ModelLoader.h Utils/ModelLoader.cpp)
list(TRANSFORM coreSources PREPEND src/Core/)

list(APPEND sources ${coreSources})
