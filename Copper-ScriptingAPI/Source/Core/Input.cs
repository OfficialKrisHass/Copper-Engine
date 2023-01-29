using System;

namespace Copper {

    public static class Input {

        public static bool IsKey(KeyCode keyCode) { return InternalCalls.IsKey((int) keyCode); }
        public static bool IsKeyDown(KeyCode keyCode) { return InternalCalls.IsKeyDown((int) keyCode); }
        public static bool IsKeyReleased(KeyCode keyCode) { return InternalCalls.IsKeyReleased((int) keyCode); }

        public static float GetAxis(string axisName) { return InternalCalls.GetAxis(axisName); }

    }

}