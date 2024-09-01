using System;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;

namespace Copper {

    [StructLayout(LayoutKind.Sequential)]
    [NativeClass("Engine/Scripting/InternalCalls/Core/Game.cs.h")]
    public static class Game {

        public static float deltaTime {

            [MethodImpl(MethodImplOptions.InternalCall)]
            [NativeFunction("GetDeltaTime")]
            get;

        }

    }
    
}
