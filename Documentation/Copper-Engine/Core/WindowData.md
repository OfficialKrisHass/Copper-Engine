# Summary
Struct storing the window data (mostly events, size, title, etc). Each window stores its own WindowData instance.

# Header
Engine/Core/Window.h
# Source
No source
# Namespace
Copper
# Type
Struct

# Functions
WindowData()#default constructor

# Variables
UVector2I size#Size of the window in pixels, always accurate.
std::string title#Title of the window.

WindowCloseEvent windowCloseEvent#This event is fired when the close button is clicked. By default the window actually closes only if this event didn't block.
WindowFocusedEvent windowFocusedEvent#This event is fired when the window either loses or gains focus, that is known by the focused variable.
WindowResizeEvent windowResizeEvent#This event is fired when the window is resized, and the new size is known via the size variable.

KeyEvent keyPressedEvent#This event is fired the frame the key gets pressed, the key is known via the key variable.
KeyEvent keyRepeatEvent#This event is fired every frame the key is held down, the key is known via the key variable. WARNING: This event is fired after a small pause from the keyPressedEvent, not immediately the next frame after!
KeyEvent keyReleasedEvent#This event is fired the frame the key gets released, the key is known via the key variable.

MouseEvent mouseMoveEvent#This event is fired when the mouse is moved, the new coords are known via the mouseCoords variable, the button variable is kept from the last button pressed/released event.
MouseEvent mouseButtonPressedEvent#This event is fired the frame the button is pressed, the button is known via the button variable, the mouseCoords variable is kept from the last mouse move event.
MouseEvent mouseButtonReleasedEvent#This event is fired the frame the button is released, the button is known via the button variable, the mouseCoords variable is kept from the last mouse move event.
