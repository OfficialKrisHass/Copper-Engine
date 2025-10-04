list(APPEND panelsSources Console.h FileBrowser.h Panel.h Properties.h SceneHierarchy.h ThemeEditor.h ProjectSettings.h Profiler.h
                          Panel.cpp Console.cpp FileBrowser.cpp Properties.cpp SceneHierarchy.cpp ThemeEditor.cpp ProjectSettings.cpp Profiler.cpp Viewport.cpp CamView.cpp)
list(TRANSFORM panelsSources PREPEND src/Panels/)

list(APPEND sources ${panelsSources})
