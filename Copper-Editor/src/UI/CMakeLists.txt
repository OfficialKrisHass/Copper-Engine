list(APPEND UISources TypeWidgets.h NewFileWindow.h
                      TypeWidgets.cpp NewFileWindow.cpp)
list(TRANSFORM UISources PREPEND src/UI/)

list(APPEND sources ${UISources})
