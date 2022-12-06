using System;
using System.Runtime.CompilerServices;

namespace Copper {

    public static class InternalCalls {

        //Logging
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void Log(string msg);
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void LogWarn(string msg);
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void LogError(string msg);

    }

}