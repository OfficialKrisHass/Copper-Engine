list(APPEND panelsSources Console.h FileBrowser.h Panel.h Properties.h SceneHierarchy.h ThemeEditor.h
                          Console.cpp FileBrowser.cpp Properties.cpp SceneHierarchy.cpp ThemeEditor.cpp)
list(TRANSFORM panelsSources PREPEND src/Panels/)

list(APPEND sources ${panelsSources})