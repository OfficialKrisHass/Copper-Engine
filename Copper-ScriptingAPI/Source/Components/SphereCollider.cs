using System;
using System.Runtime.InteropServices;
using System.Runtime.CompilerServices;

namespace Copper {
    
    [StructLayout(LayoutKind.Sequential)]
    [NativeClass("Engine/Scripting/InternalCalls/Components/SphereCollider.cs.h")]
    public class SphereCollider : Collider {

        private static uint ComponentID() { return 8; }

        public extern float radius {

            [MethodImpl(MethodImplOptions.InternalCall)]
            [NativeFunction("GetRadius")]
            get;
            [MethodImpl(MethodImplOptions.InternalCall)]
            [NativeFunction("SetRadius")]
            set;

        }

    }
    
}
