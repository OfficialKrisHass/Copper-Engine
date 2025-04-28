# Window
Wrapper class for the window implementation, platform indenpendant.

## Details
Header: Engine/Core/Window.h
Source: Platform/<Platform>/<Platform>Window.cpp
Namespace: Copper
Type: Class

## Functions
Window() - default constructor, no window is created.
Window(const std::stirng& title, uint32 width, uint32 height, bool maximize = true) - Creates a new window with the given title, dimensions, and maximizes it if needed. Check out Initialize(const std::stirng& title, uint32 width, uint32 height, bool maximize = true) as well.

Initialize(const std::stirng& title, uint32 width, uint32 height, bool maximize = true) - Initializes a new window into an already existing Window class instance. Used to prevent creating a window and copying it (windowVar = Window(title, width, height), instead, windowVar.Initialize(title, width, height)) 
void Update() - Updates the underlying window, swaps the buffers, polls events, etc.
void Shutdown() - Shutdowns the window and destroys the underlying window instance.

void SetAsCurrentContext() - Sets this window as the current window, all the following render calls will render into this window.

float GetTime() - Returns the time since the window was created.

uint32 GetWidth() - Returns the width of the window.
uint32 GetHeight() - Returns the height of the window.
const UVector2I& GetSize() - Returns the size of the window as a UVector2I.
float GetAspectRatio() - Calculates and returns the aspect ratio (width / height).

const std::string& GetTitle() - Returns the window title.

WindowCloseEvent& GetWindowCloseEvent() - Returns a reference to the window close event.
WindowFocused& GetWindowFocusedEvent() - Returns a reference to the window focused event.
WindowResize& GetWindowResizeEvent() - Returns a reference to the window resize event.

KeyEvent& GetKeyPressedEvent() - Returns a reference to the key pressed event.
KeyEvent& GetKeyRepeatEvent() - Returns a reference to the key repeat event.
KeyEvent& GetKeyReleasedEvent() - Returns a reference to the key released event.

MouseEvent& GetMouseMoveEvent() - Returns a reference to the mouse move event.
MouseEvent& GetMouseButtonPressedEvent() - Returns a reference to the mouse button pressed event.
MouseEvent& GetMouseButtonReleasedEvent() - Returns a reference to the mouse button released event.

void SetSize(const UVector2I& size) - Sets the size of the window to size.

void SetTitle(const std::string& title) - Sets the title of the window.
