list(APPEND UISources TypeWidgets.h NewItemModal.h LoadingBarModal.h
                      TypeWidgets.cpp NewItemModal.cpp LoadingBarModal.cpp)
list(TRANSFORM UISources PREPEND src/UI/)

list(APPEND sources ${UISources})
