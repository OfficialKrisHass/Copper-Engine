list(APPEND assetFileSources AssetFileDatabase.h Deserializer.h
							 AssetFileDatabase.cpp Deserializer.cpp)
list(TRANSFORM assetFileSources PREPEND src/AssetFile/)

list(APPEND sources ${assetFileSources})