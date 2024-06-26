using System;
using System.Runtime.InteropServices;
using System.Runtime.CompilerServices;

namespace Copper {
    
    [StructLayout(LayoutKind.Sequential)]
    [NativeClass("Engine/Scripting/InternalCalls/Components/BoxCollider.cs.h")]
    public class BoxCollider : Collider {

        private static uint ComponentID() { return 7; }

        public extern Vector3 size {

            [MethodImpl(MethodImplOptions.InternalCall)]
            [NativeFunction("GetSize")]
            get;
            [MethodImpl(MethodImplOptions.InternalCall)]
            [NativeFunction("SetSize")]
            set;

        }

    }
    
}
