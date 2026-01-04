list(APPEND coreSources Core.h Engine.h Entry.h Log.h Window.h Args.h UUID.h SignalHandler.h
                        Args.cpp Engine.cpp Log.cpp UUID.cpp UUID_SSE4.cpp UUID_AVX2.cpp SignalHandler.cpp)
list(TRANSFORM coreSources PREPEND src/Engine/Core/)

list(APPEND sources ${coreSources})

set_source_files_properties(src/Engine/Core/UUID_SSE4.cpp PROPERTIES COMPILE_OPTIONS "-msse4.1" SKIP_PRECOMPILE_HEADERS ON)
set_source_files_properties(src/Engine/Core/UUID_AVX2.cpp PROPERTIES COMPILE_OPTIONS "-mavx2" SKIP_PRECOMPILE_HEADERS ON)
