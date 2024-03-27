list(APPEND physicsSources PhysicsEngine.h Raycast.h
                           PhysicsEngine.cpp CollisionNotifier.cpp PhysicsSimulationShader.cpp Collider.cpp RigidBody.cpp ScenePhysics.cpp Raycast.cpp)
list(TRANSFORM physicsSources PREPEND src/Engine/Physics/)

list(APPEND sources ${physicsSources})
