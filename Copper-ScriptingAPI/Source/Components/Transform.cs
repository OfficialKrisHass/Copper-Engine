using System;
using System.Runtime.InteropServices;
using System.Runtime.CompilerServices;

namespace Copper {

    [StructLayout(LayoutKind.Sequential)]
    public class Transform : Component {

        public Vector3 position {

            [MethodImpl(MethodImplOptions.InternalCall)]
            get;
            [MethodImpl(MethodImplOptions.InternalCall)]
            set;

        }
        public Vector3 scale {

            [MethodImpl(MethodImplOptions.InternalCall)]
            get;
            [MethodImpl(MethodImplOptions.InternalCall)]
            set;

        }

        public Vector3 forward {

            [MethodImpl(MethodImplOptions.InternalCall)]
            get;

        }
        public Vector3 right {

            [MethodImpl(MethodImplOptions.InternalCall)]
            get;

        }
        public Vector3 up {

            [MethodImpl(MethodImplOptions.InternalCall)]
            get;

        }

        public Vector3 back { get { return -forward; } }
        public Vector3 left { get { return -right; } }
        public Vector3 down { get { return -down; } }

    }

}
