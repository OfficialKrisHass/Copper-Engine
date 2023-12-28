list(APPEND componentsSources Camera.h Light.h MeshRenderer.h ScriptComponent.h Transform.h RigidBody.h BoxCollider.h
                              Camera.cpp ScriptComponent.cpp Transform.cpp)
list(TRANSFORM componentsSources PREPEND src/Engine/Components/)

list(APPEND sources ${componentsSources})