list(APPEND UISources TypeWidgets.h NewItemModal.h
                      TypeWidgets.cpp NewItemModal.cpp)
list(TRANSFORM UISources PREPEND src/UI/)

list(APPEND sources ${UISources})
