using System;
using System.Runtime.InteropServices;
using System.Runtime.CompilerServices;

namespace Copper {

    [StructLayout(LayoutKind.Sequential)]
    [NativeClass("Engine/Scripting/InternalCalls/Components/Light.cs.h")]
    public class Light : Component {

       private static uint ComponentID() { return 3; } 

        public enum Type {

            Point = 0,
            Directional = 1

        }

        public extern Color color {

            [MethodImpl(MethodImplOptions.InternalCall)]
            [NativeFunction("GetColor")]
            get;
            [MethodImpl(MethodImplOptions.InternalCall)]
            [NativeFunction("SetColor")]
            set;

        }
        public extern Type type {

            [MethodImpl(MethodImplOptions.InternalCall)]
            [NativeFunction("GetType")]
            get;
            [MethodImpl(MethodImplOptions.InternalCall)]
            [NativeFunction("SetType")]
            set;

        }

        public extern float intensity {

            [MethodImpl(MethodImplOptions.InternalCall)]
            [NativeFunction("GetIntensity")]
            get;
            [MethodImpl(MethodImplOptions.InternalCall)]
            [NativeFunction("SetIntensity")]
            set;

        }

    }

}
