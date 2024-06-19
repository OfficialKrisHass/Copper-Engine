using System;
using System.Runtime.InteropServices;
using System.Runtime.CompilerServices;

namespace Copper {

    [StructLayout(LayoutKind.Sequential)]
    [NativeClass]
    public class Light : Component {

        public static int ComponentID() { return 3; } 

        public enum Type {

            Point = 0,
            Directional = 1

        }

        public extern Color color {

            [MethodImpl(MethodImplOptions.InternalCall)]
            get;
            [MethodImpl(MethodImplOptions.InternalCall)]
            set;

        }
        public extern Type type {

            [MethodImpl(MethodImplOptions.InternalCall)]
            get;
            [MethodImpl(MethodImplOptions.InternalCall)]
            set;

        }

        public extern float intensity {

            [MethodImpl(MethodImplOptions.InternalCall)]
            get;
            [MethodImpl(MethodImplOptions.InternalCall)]
            set;

        }

    }

}
