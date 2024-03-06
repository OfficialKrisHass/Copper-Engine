list(APPEND assetStorageSources AssetPtr.h AssetMap.h AssetStorage.h
								AssetStorage.cpp)
list(TRANSFORM assetStorageSources PREPEND src/Engine/AssetStorage/)

list(APPEND sources ${assetStorageSources})