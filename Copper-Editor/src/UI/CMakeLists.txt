list(APPEND UISources TypeWidgets.h Widgets.h NewItemModal.h LoadingBarModal.h
                      TypeWidgets.cpp Widgets.cpp NewItemModal.cpp LoadingBarModal.cpp)
list(TRANSFORM UISources PREPEND src/UI/)

list(APPEND sources ${UISources})
