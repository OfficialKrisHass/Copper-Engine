using System;
using System.Runtime.InteropServices;
using System.Runtime.CompilerServices;

namespace Copper {

    [StructLayout(LayoutKind.Sequential)]
    [NativeClass("Engine/Scripting/InternalCalls/Renderer/Material.cs.h")]
    public class Material : Asset {

        public Color albedo {

            [MethodImpl(MethodImplOptions.InternalCall)]
            [NativeFunction("GetAlbedo")]
            get;
            [MethodImpl(MethodImplOptions.InternalCall)]
            [NativeFunction("SetAlbedo")]
            set;

        }
        public float tiling {

            [MethodImpl(MethodImplOptions.InternalCall)]
            [NativeFunction("GetTiling")]
            get;
            [MethodImpl(MethodImplOptions.InternalCall)]
            [NativeFunction("SetTiling")]
            set;

        }

    }

}
