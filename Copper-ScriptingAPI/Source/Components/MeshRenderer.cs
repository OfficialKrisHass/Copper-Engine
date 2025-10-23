using System;
using System.Runtime.InteropServices;
using System.Runtime.CompilerServices;

namespace Copper {

    [StructLayout(LayoutKind.Sequential)]
    [NativeClass("Engine/Scripting/InternalCalls/Components/MeshRenderer.cs.h")]
    public class MeshRenderer : Component {

        private static uint ComponentID() { return 1; }

        public extern Material material {

            [MethodImpl(MethodImplOptions.InternalCall)]
            [NativeFunction("GetMaterial")]
            get;
            [MethodImpl(MethodImplOptions.InternalCall)]
            [NativeFunction("SetMaterial")]
            set;

        }

    }

}
