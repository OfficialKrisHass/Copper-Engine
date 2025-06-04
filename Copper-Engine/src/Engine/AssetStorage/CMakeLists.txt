list(APPEND assetStorageSources AssetPtr.h AssetMap.h AssetStorage.h)
list(TRANSFORM assetStorageSources PREPEND src/Engine/AssetStorage/)

list(APPEND sources ${assetStorageSources})
