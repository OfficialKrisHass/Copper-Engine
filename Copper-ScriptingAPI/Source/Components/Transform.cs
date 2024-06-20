using System;
using System.Runtime.InteropServices;
using System.Runtime.CompilerServices;

namespace Copper {

    [StructLayout(LayoutKind.Sequential)]
    [NativeClass("Engine/Scripting/InternalCalls/Components/Transform.cs.h")]
    public class Transform : Component {

        public static int ComponentID() { return 0; }

        public extern Vector3 position {

            [MethodImpl(MethodImplOptions.InternalCall)]
            [NativeFunction("GetPosition")]
            get;
            [MethodImpl(MethodImplOptions.InternalCall)]
            [NativeFunction("SetPosition")]
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
            [NativeFunction("GetScale")]
            get;
            [MethodImpl(MethodImplOptions.InternalCall)]
            [NativeFunction("SetScale")]
            set;

        }

        public extern Vector3 globalPosition {

            [MethodImpl(MethodImplOptions.InternalCall)]
            [NativeFunction("GetGlobalPosition")]
            get;

        }
        public Quaternion globalRotation {

            get {

                Quaternion ret;
                Internal_GetGlobalRotation(this, out ret);
                return ret;

            }

        }
        public extern Vector3 globalScale {

            [MethodImpl(MethodImplOptions.InternalCall)]
            [NativeFunction("GetGlobalScale")]
            get;

        }

        public Vector3 eulerAngles {

            get { return rotation.eulerAngles; }
            set { rotation = new Quaternion(value); }

        }
        public Vector3 globalEulerAngles {

            get { return globalRotation.eulerAngles; }

        }

        public Vector3 forward {

            [MethodImpl(MethodImplOptions.InternalCall)]
            [NativeFunction("GetForward")]
            get;

        }
        public Vector3 right {

            [MethodImpl(MethodImplOptions.InternalCall)]
            [NativeFunction("GetRight")]
            get;

        }
        public Vector3 up {

            [MethodImpl(MethodImplOptions.InternalCall)]
            [NativeFunction("GetUp")]
            get;

        }

        public Vector3 back { get { return -forward; } }
        public Vector3 left { get { return -right; } }
        public Vector3 down { get { return -down; } }

        [MethodImpl(MethodImplOptions.InternalCall)]
        [NativeFunction("GetRotation")]
        internal extern static void Internal_GetRotation(Transform transform, out Quaternion ret);
        [MethodImpl(MethodImplOptions.InternalCall)]
        [NativeFunction("SetRotation")]
        internal extern static void Internal_SetRotation(Transform transform, ref Quaternion value);
        
        [MethodImpl(MethodImplOptions.InternalCall)]
        [NativeFunction("GetGlobalRotation")]
        internal extern static void Internal_GetGlobalRotation(Transform transform, out Quaternion ret);

    }

}
