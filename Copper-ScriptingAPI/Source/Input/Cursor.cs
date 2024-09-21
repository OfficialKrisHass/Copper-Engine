using System;
using System.Runtime.InteropServices;
using System.Runtime.CompilerServices;

namespace Copper {

    [NativeClass("Engine/Scripting/InternalCalls/Input/Cursor.cs.h")]
    public static class Cursor {

        public static bool visible {

            [MethodImpl(MethodImplOptions.InternalCall)]
            [NativeFunction("IsVisible")]
            get;
            [MethodImpl(MethodImplOptions.InternalCall)]
            [NativeFunction("SetVisible")]
            set; 

        }
        public static bool locked {

            [MethodImpl(MethodImplOptions.InternalCall)]
            [NativeFunction("IsLocked")]
            get;
            [MethodImpl(MethodImplOptions.InternalCall)]
            [NativeFunction("SetLocked")]
            set; 

        }

        public static Vector2 position {

            [MethodImpl(MethodImplOptions.InternalCall)]
            [NativeFunction("GetPosition")]
            get;
            [MethodImpl(MethodImplOptions.InternalCall)]
            [NativeFunction("SetPosition")]
            set;

        }

    }

}
