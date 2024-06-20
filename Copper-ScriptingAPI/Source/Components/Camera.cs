using System;
using System.Runtime.InteropServices;
using System.Runtime.CompilerServices;

namespace Copper {

    [StructLayout(LayoutKind.Sequential)]
    [NativeClass("Engine/Scripting/InternalCalls/Components/Camera.cs.h")]
    public class Camera : Component {

        public static int ComponentID() { return 2; } 

        public extern float fov {

            [MethodImpl(MethodImplOptions.InternalCall)]
            [NativeFunction("GetFov")]
            get;
            [MethodImpl(MethodImplOptions.InternalCall)]
            [NativeFunction("SetFov")]
            set;

        }

        public float nearPlane {

            [MethodImpl(MethodImplOptions.InternalCall)]
            [NativeFunction("GetNearPlane")]
            get;
            [MethodImpl(MethodImplOptions.InternalCall)]
            [NativeFunction("SetNearPlane")]
            set;

        }
        public float farPlane {

            [MethodImpl(MethodImplOptions.InternalCall)]
            [NativeFunction("GetFarPlane")]
            get;
            [MethodImpl(MethodImplOptions.InternalCall)]
            [NativeFunction("SetFarPlane")]
            set;

        }

    }

}
