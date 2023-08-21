list(APPEND yamlOverloadsSources Color.h Entity.h Path.h Vectors.h Everything.h)
list(TRANSFORM yamlOverloadsSources PREPEND src/Engine/YAMLOverloads/)

list(APPEND sources ${yamlOverloadsSources})