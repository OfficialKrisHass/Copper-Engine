list(APPEND sceneSources ComponentView.h CopperECS.h Entity.h EntityView.h InternalEntity.h OldSceneDeserialization.h Registry.h Scene.h InternalEntity.inl
                         Entity.cpp OldSceneDeserialization.cpp Registry.cpp Scene.cpp)
list(TRANSFORM sceneSources PREPEND src/Engine/Scene/)

list(APPEND sources ${sceneSources})
