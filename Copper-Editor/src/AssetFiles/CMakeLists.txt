list(APPEND assetFilesSources AssetFileDatabase.h
							  AssetFileDatabase.cpp)
list(TRANSFORM assetFilesSources PREPEND src/AssetFiles/)

list(APPEND sources ${assetFilesSources})