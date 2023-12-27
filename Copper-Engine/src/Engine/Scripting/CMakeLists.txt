list(APPEND scriptingSources Field.h MonoUtils.h ScriptingCore.h InternalCalls.h
                             MonoUtils.cpp ScriptingCore.cpp InternalCalls.cpp
                             Editor.cs.cpp Input.cs.cpp Quaternion.cs.cpp Entity.cs.cpp Component.cs.cpp Transform.cs.cpp Camera.cs.cpp RigidBody.cs.cpp BoxCollider.cs.cpp)
list(TRANSFORM scriptingSources PREPEND src/Engine/Scripting/)

list(APPEND sources ${scriptingSources})