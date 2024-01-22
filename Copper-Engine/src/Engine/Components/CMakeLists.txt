list(APPEND componentsSources Component.h Camera.h Light.h MeshRenderer.h ScriptComponent.h Transform.h RigidBody.h BoxCollider.h SphereCollider.h CapsuleCollider.h
                              Camera.cpp ScriptComponent.cpp Transform.cpp)
list(TRANSFORM componentsSources PREPEND src/Engine/Components/)

list(APPEND sources ${componentsSources})