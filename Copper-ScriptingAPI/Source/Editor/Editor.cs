using System;

namespace Copper {

    public static class Editor {

        public static void Log(string msg) { InternalCalls.Log(msg); }
        public static void LogWarn(string msg) { InternalCalls.LogWarn(msg); }
        public static void LogError(string msg) { InternalCalls.LogError(msg); }

    }

}