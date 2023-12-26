using System;
using System.Runtime.CompilerServices;

namespace Copper {

    public static class Editor {

        public static void Log(string msg) { Internal_Log(msg); }
        public static void LogWarn(string msg) { Internal_LogWarn(msg); }
        public static void LogError(string msg) { Internal_LogError(msg); }

        // Internal Functions

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void Internal_Log(string msg);
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void Internal_LogWarn(string msg);
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void Internal_LogError(string msg);

    }

}