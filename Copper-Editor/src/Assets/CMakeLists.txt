list(APPEND assetsSources ProjectAssetDatabase.h Serializer.h AssetMeta.h
						  ProjectAssetDatabase.cpp Serializer.cpp AssetMeta.cpp)
list(TRANSFORM assetsSources PREPEND src/Assets/)

list(APPEND sources ${assetsSources})