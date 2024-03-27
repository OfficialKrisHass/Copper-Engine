list(APPEND debugSources Assert.h Profiler.h
						 Profiler.cpp)
list(TRANSFORM debugSources PREPEND src/Debug/)

list(APPEND sources ${debugSources})
