list(APPEND debugSources Assert.h Profiler.h
						 Profiler.cpp)
list(TRANSFORM debugSources PREPEND src/Engine/Debug/)

list(APPEND sources ${debugSources})
