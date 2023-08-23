list(APPEND coreSources Core.h Engine.h Entry.h Log.h Window.h
                        Engine.cpp Log.cpp)
list(TRANSFORM coreSources PREPEND src/Engine/Core/)

list(APPEND sources ${coreSources})