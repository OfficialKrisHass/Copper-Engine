using System;

namespace Copper {

    public static class Input {

        public static bool IsKey(KeyCode keyCode) { return InternalCalls.IsKey((int) keyCode); }
        public static bool IsKey(int keyCode) { return InternalCalls.IsKey(keyCode); }

        public static float GetAxis(string axisName) { return InternalCalls.GetAxis(axisName); }

    }

}