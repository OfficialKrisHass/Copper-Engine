list(APPEND assetsSources AssetType.h ProjectAssetDatabase.h Serializer.h
                          AssetType.cpp ProjectAssetDatabase.cpp Serializer.cpp Model.cpp)
list(TRANSFORM assetsSources PREPEND src/Assets/)

list(APPEND sources ${assetsSources})
