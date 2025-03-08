list(APPEND UISources TypeWidgets.h NewModal.h
                      TypeWidgets.cpp NewModal.cpp)
list(TRANSFORM UISources PREPEND src/UI/)

list(APPEND sources ${UISources})
