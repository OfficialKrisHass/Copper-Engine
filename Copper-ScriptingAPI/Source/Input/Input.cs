using System;
using System.Runtime.InteropServices;
using System.Runtime.CompilerServices;

namespace Copper {

    [NativeClass("Engine/Scripting/InternalCalls/Input/Input.cs.h")]
    public static class Input {

        // Getting input
        
        public static KeyState GetKeyState(KeyCode key) { return Internal_GetKeyState(key); }

        // Input axes

        public static float GetAxis(string axis) { return Internal_GetAxis(axis); }

        [MethodImpl(MethodImplOptions.InternalCall)]
        [NativeFunction("GetKeyState")]
        internal extern static KeyState Internal_GetKeyState(KeyCode key);

        [MethodImpl(MethodImplOptions.InternalCall)]
        [NativeFunction("GetAxis")]
        internal extern static float Internal_GetAxis(string axis);

    }

}
