list(APPEND assetsSources ProjectAssetDatabase.h Serializer.h
                          ProjectAssetDatabase.cpp Serializer.cpp Model.cpp)
list(TRANSFORM assetsSources PREPEND src/Assets/)

list(APPEND sources ${assetsSources})
