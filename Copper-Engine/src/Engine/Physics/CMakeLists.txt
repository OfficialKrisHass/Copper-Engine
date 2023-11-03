list(APPEND physicsSources PhysicsEngine.h
                           PhysicsEngine.cpp CollisionNotifier.cpp PhysicsSimulationShader.cpp)
list(TRANSFORM physicsSources PREPEND src/Engine/Physics/)

list(APPEND sources ${physicsSources})