list(APPEND assetFileSources AssetFileDatabase.h Serializer.h MetaFile.h
							 AssetFileDatabase.cpp Serializer.cpp MetaFile.cpp)
list(TRANSFORM assetFileSources PREPEND src/AssetFile/)

list(APPEND sources ${assetFileSources})