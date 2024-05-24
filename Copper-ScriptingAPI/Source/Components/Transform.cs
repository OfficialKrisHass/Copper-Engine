using System;
using System.Runtime.InteropServices;
using System.Runtime.CompilerServices;

namespace Copper {

    [StructLayout(LayoutKind.Sequential)]
    public class Transform : Component {

        public extern Vector3 position {

            [MethodImpl(MethodImplOptions.InternalCall)]
            get;
            [MethodImpl(MethodImplOptions.InternalCall)]
            set;

        }
        public Quaternion rotation {

            get {

                Quaternion ret;
                Internal_GetRotation(this, out ret);
                return ret;

            }
            set { Internal_SetRotation(this, ref value); }

        }
        public extern Vector3 scale {

            [MethodImpl(MethodImplOptions.InternalCall)]
            get;
            [MethodImpl(MethodImplOptions.InternalCall)]
            set;

        }

        public Vector3 eulerAngles {

            get { return rotation.eulerAngles; }
            set { rotation = new Quaternion(value); }

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

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void Internal_GetRotation(Transform transform, out Quaternion ret);
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void Internal_SetRotation(Transform transform, ref Quaternion value);

    }

}
