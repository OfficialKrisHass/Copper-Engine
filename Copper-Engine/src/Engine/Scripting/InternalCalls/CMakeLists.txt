list(APPEND scriptingInternalCallsSources
	 Editor.cs.cpp Game.cs.cpp Input.cs.cpp Quaternion.cs.cpp Entity.cs.cpp Component.cs.cpp Transform.cs.cpp Camera.cs.cpp RigidBody.cs.cpp Collider.cs.cpp, Raycast.cs.cpp)
list(TRANSFORM scriptingInternalCallsSources PREPEND src/Engine/Scripting/InternalCalls/)

list(APPEND sources ${scriptingInternalCallsSources)