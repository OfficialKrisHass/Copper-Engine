list(APPEND UISources TypeWidgets.h
                      TypeWidgets.cpp)
list(TRANSFORM UISources PREPEND src/UI/)

list(APPEND sources ${UISources})
