list(APPEND physicsSources PhysicsEngine.h
                           PhysicsEngine.cpp CollisionNotifier.cpp PhysicsSimulationShader.cpp Collider.cpp RigidBody.cpp ScenePhysics.cpp)
list(TRANSFORM physicsSources PREPEND src/Engine/Physics/)

list(APPEND sources ${physicsSources})