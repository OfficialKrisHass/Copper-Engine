list(APPEND eventsSources Event.h KeyEvent.h MouseEvent.h RegistryEvent.h WindowEvent.h)
list(TRANSFORM eventsSources PREPEND src/Engine/Events/)

list(APPEND sources ${eventsSources})
