using System;
using System.Runtime.CompilerServices;

namespace Copper {

    public static class Input {

        public static bool IsKey(KeyCode keyCode) { return Internal_IsKey((int) keyCode); }
        public static bool IsKeyDown(KeyCode keyCode) { return Internal_IsKeyDown((int) keyCode); }
        public static bool IsKeyReleased(KeyCode keyCode) { return Internal_IsKeyReleased((int) keyCode); }

        public static float GetAxis(string axisName) { return Internal_GetAxis(axisName); }

        public static void SetCursorVisible(bool visible) { Internal_SetCursorVisible(visible); }
        public static void SetCursorLocked(bool locked) { Internal_SetCursorLocked(locked); }

        // Internal Functions

        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static bool Internal_IsKey(int keyCode);
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static bool Internal_IsKeyDown(int keyCode);
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static bool Internal_IsKeyReleased(int keyCode);
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static float Internal_GetAxis(string axisName);

        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static void Internal_SetCursorVisible(bool visible);
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static void Internal_SetCursorLocked(bool locked);

    }

}