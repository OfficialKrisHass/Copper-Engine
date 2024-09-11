using System;
using System.Runtime.InteropServices;
using System.Runtime.CompilerServices;

namespace Copper {

    [NativeClass("Engine/Scripting/InternalCalls/Input/Input.cs.h")]
    public static class Input {

        public static bool IsKey(KeyCode key) { return Internal_IsKey(key); }
        public static bool IsKeyDown(KeyCode key) { return Internal_IsKeyDown(key); }
        public static bool IsKeyReleased(KeyCode key) { return Internal_IsKeyReleased(key); }

        public static bool IsButton(MouseCode button) { return Internal_IsButton(button); }

        public static float GetAxis(string axis) { return Internal_GetAxis(axis); }

        [MethodImpl(MethodImplOptions.InternalCall)]
        [NativeFunction("IsKey")]
        internal extern static bool Internal_IsKey(KeyCode key);
        [MethodImpl(MethodImplOptions.InternalCall)]
        [NativeFunction("IsKeyDown")]
        internal extern static bool Internal_IsKeyDown(KeyCode key);
        [MethodImpl(MethodImplOptions.InternalCall)]
        [NativeFunction("IsKeyReleased")]
        internal extern static bool Internal_IsKeyReleased(KeyCode key);

        [MethodImpl(MethodImplOptions.InternalCall)]
        [NativeFunction("IsButton")]
        internal extern static bool Internal_IsButton(MouseCode button);

        [MethodImpl(MethodImplOptions.InternalCall)]
        [NativeFunction("GetAxis")]
        internal extern static float Internal_GetAxis(string axis);

    }

}
