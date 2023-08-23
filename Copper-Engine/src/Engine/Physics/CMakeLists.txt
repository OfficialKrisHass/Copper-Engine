list(APPEND physicsSources PhysicsEngine.h
                           PhysicsEngine.cpp)
list(TRANSFORM physicsSources PREPEND src/Engine/Physics/)

list(APPEND sources ${physicsSources})