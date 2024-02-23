list(APPEND assetStorageSources AssetStorage.h AssetList.h
								AssetStorage.cpp)
list(TRANSFORM assetStorageSources PREPEND src/Engine/AssetStorage/)

list(APPEND sources ${assetStorageSources})