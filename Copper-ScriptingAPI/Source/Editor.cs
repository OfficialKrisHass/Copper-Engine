using System;

namespace Copper {

    public static class Editor {

        public static void Log(string msg) { InternalFunctions.Log(msg); }
        public static void LogWarn(string msg) { InternalFunctions.LogWarn(msg); }
        public static void LogError(string msg) { InternalFunctions.LogError(msg); }

    }

}