list(APPEND componentsSources Component.h Camera.h Light.h MeshRenderer.h Transform.h RigidBody.h BoxCollider.h SphereCollider.h CapsuleCollider.h
                              Camera.cpp Transform.cpp ScriptComponent.cpp RigidBody.cpp Collider.cpp BoxCollider.cpp SphereCollider.cpp CapsuleCollider.cpp)
list(TRANSFORM componentsSources PREPEND src/Engine/Components/)

list(APPEND sources ${componentsSources})
