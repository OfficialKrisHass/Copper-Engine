list(APPEND yamlOverloadsSources Color.h Entity.h Path.h Vectors.h UUID.h AssetPtr.h Everything.h)
list(TRANSFORM yamlOverloadsSources PREPEND src/Engine/YAMLOverloads/)

list(APPEND sources ${yamlOverloadsSources})