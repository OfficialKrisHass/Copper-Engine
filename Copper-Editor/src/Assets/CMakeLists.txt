list(APPEND assetsSources ProjectAssetDatabase.h Serializer.h MetaFile.h
						  ProjectAssetDatabase.cpp Serializer.cpp MetaFile.cpp)
list(TRANSFORM assetsSources PREPEND src/Assets/)

list(APPEND sources ${assetsSources})