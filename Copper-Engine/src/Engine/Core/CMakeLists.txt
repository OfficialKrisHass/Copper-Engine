list(APPEND coreSources Core.h Engine.h Entry.h Log.h Window.h Args.h UUID.h SignalHandler.h
                        Args.cpp Engine.cpp Log.cpp UUID.cpp SignalHandler.cpp)
list(TRANSFORM coreSources PREPEND src/Engine/Core/)

list(APPEND sources ${coreSources})
