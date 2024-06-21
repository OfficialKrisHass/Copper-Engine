list(APPEND icallComponentsSources Component.cs.cpp Transform.cs.cpp Camera.cs.cpp Light.cs.cpp RigidBody.cs.cpp Collider.cs.cpp)
list(TRANSFORM icallComponentsSources PREPEND src/Engine/Scripting/InternalCalls/Components/)

list(APPEND sources ${icallComponentsSources})
