list(APPEND assetStorageSources AssetStorage.h AssetMap.h
								AssetStorage.cpp)
list(TRANSFORM assetStorageSources PREPEND src/Engine/AssetStorage/)

list(APPEND sources ${assetStorageSources})