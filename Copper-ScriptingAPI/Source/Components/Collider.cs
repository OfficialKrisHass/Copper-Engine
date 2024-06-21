using System;
using System.Runtime.InteropServices;
using System.Runtime.CompilerServices;

namespace Copper {

    [StructLayout(LayoutKind.Sequential)]
    [NativeClass("Engine/Scripting/InternalCalls/Components/Collider.cs.h")]
    public class Collider : Component {

        private static uint cID = 6;

        public enum Type : byte {

            None = 0,

            Box,
            Sphere,
            Capsule,

        }

        public extern Type type {

            [MethodImpl(MethodImplOptions.InternalCall)]
            [NativeFunction("GetType")]
            get;

        }
        public extern RigidBody rigidBody {

            [MethodImpl(MethodImplOptions.InternalCall)]
            [NativeFunction("GetRigidBody")]
            get;

        }

        public extern bool trigger {

            [MethodImpl(MethodImplOptions.InternalCall)]
            [NativeFunction("GetTrigger")]
            get;
            [MethodImpl(MethodImplOptions.InternalCall)]
            [NativeFunction("SetTrigger")]
            set;

        }
        public extern Vector3 center {

            [MethodImpl(MethodImplOptions.InternalCall)]
            [NativeFunction("GetCenter")]
            get;
            [MethodImpl(MethodImplOptions.InternalCall)]
            [NativeFunction("SetCenter")]
            set;

        }

    }

}
