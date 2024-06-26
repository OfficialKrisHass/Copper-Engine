using System;
using System.Runtime.InteropServices;
using System.Runtime.CompilerServices;

namespace Copper {
    
    [StructLayout(LayoutKind.Sequential)]
    [NativeClass("Engine/Scripting/InternalCalls/Components/CapsuleCollider.cs.h")]
    public class CapsuleCollider : Collider {

        private static uint ComponentID() { return 9; }

        public extern float radius {

            [MethodImpl(MethodImplOptions.InternalCall)]
            [NativeFunction("GetRadius")]
            get;
            [MethodImpl(MethodImplOptions.InternalCall)]
            [NativeFunction("SetRadius")]
            set;

        }
        public extern float height {

            [MethodImpl(MethodImplOptions.InternalCall)]
            [NativeFunction("GetHeight")]
            get;
            [MethodImpl(MethodImplOptions.InternalCall)]
            [NativeFunction("SetHeight")]
            set;

        }

    }
    
}
