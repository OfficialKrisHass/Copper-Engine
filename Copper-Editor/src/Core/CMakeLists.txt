list(APPEND coreSources EditorApp.h ProjectFileWatcher.h SceneMeta.h AssetFileDatabase.h
                        EditorApp.cpp ProjectFileWatcher.cpp SceneMeta.cpp AssetFileDatabase.cpp
                        Utils/ModelLoader.h Utils/ModelLoader.cpp)
list(TRANSFORM coreSources PREPEND src/Core/)

list(APPEND sources ${coreSources})