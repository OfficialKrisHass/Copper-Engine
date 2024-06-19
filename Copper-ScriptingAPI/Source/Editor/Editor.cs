using System;
using System.Runtime.CompilerServices;

namespace Copper {
    
    [NativeClass]
    public static class Editor {
        
        public static void Log(string msg) { Internal_EditorLog(msg); }
        public static void LogWarn(string msg) { Internal_EditorLogWarn(msg); }
        public static void LogError(string msg) { Internal_EditorLogError(msg); }

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void Internal_EditorLog(string msg);
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void Internal_EditorLogWarn(string msg);
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void Internal_EditorLogError(string msg);

    }

}
