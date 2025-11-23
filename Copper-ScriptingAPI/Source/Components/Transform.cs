using System;
using System.Runtime.InteropServices;
using System.Runtime.CompilerServices;

namespace Copper {

    [StructLayout(LayoutKind.Sequential)]
    [NativeClass("Engine/Scripting/InternalCalls/Components/Transform.cs.h")]
    public class Transform : Component {

        private static uint ComponentID() { return 0; } 

        // Position, Rotation and Scale

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
            [MethodImpl(MethodImplOptions.InternalCall)]
            [NativeFunction("SetGlobalPosition")]
            set;

        }
        public Quaternion globalRotation {

            get {

                Quaternion ret;
                Internal_GetGlobalRotation(this, out ret);
                return ret;

            }
            set { Internal_SetGlobalRotation(this, ref value); }

        }
        public extern Vector3 globalScale {

            [MethodImpl(MethodImplOptions.InternalCall)]
            [NativeFunction("GetGlobalScale")]
            get;
            [MethodImpl(MethodImplOptions.InternalCall)]
            [NativeFunction("SetGlobalScale")]
            set;

        }

        public Vector3 eulerAngles {

            get { return rotation.eulerAngles; }
            set { rotation = new Quaternion(value); }

        }
        public Vector3 globalEulerAngles {

            get { return globalRotation.eulerAngles; }
            set { globalRotation = new Quaternion(value); }

        }

        // Directions

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

        // Child & Parent
        
        public Transform parent {

            [MethodImpl(MethodImplOptions.InternalCall)]
            [NativeFunction("GetParent")]
            get;
            [MethodImpl(MethodImplOptions.InternalCall)]
            [NativeFunction("SetParent")]
            set;

        }

        public Transform GetChild(uint index) { return Internal_GetChild(this, index); }

        public void AddChild(Transform child) { Internal_AddChild(this, child); }
        public void RemoveChild(uint index) { Internal_RemoveChild(this, index); }
        public void RemoveChild(Transform child) { Internal_RemoveChildTransform(this, child); }

        public uint childCount {

            [MethodImpl(MethodImplOptions.InternalCall)]
            [NativeFunction("GetChildCount")]
            get;

        }

        // Internal functions

        [MethodImpl(MethodImplOptions.InternalCall)]
        [NativeFunction("GetRotation")]
        internal extern static void Internal_GetRotation(Transform transform, out Quaternion ret);
        [MethodImpl(MethodImplOptions.InternalCall)]
        [NativeFunction("SetRotation")]
        internal extern static void Internal_SetRotation(Transform transform, ref Quaternion value);
        
        [MethodImpl(MethodImplOptions.InternalCall)]
        [NativeFunction("GetGlobalRotation")]
        internal extern static void Internal_GetGlobalRotation(Transform transform, out Quaternion ret);
        [MethodImpl(MethodImplOptions.InternalCall)]
        [NativeFunction("SetGlobalRotation")]
        internal extern static void Internal_SetGlobalRotation(Transform transform, ref Quaternion value);

        [MethodImpl(MethodImplOptions.InternalCall)]
        [NativeFunction("GetChild")]
        internal extern static Transform Internal_GetChild(Transform transform, uint index);

        [MethodImpl(MethodImplOptions.InternalCall)]
        [NativeFunction("AddChild")]
        internal extern static void Internal_AddChild(Transform transform, Transform child);
        [MethodImpl(MethodImplOptions.InternalCall)]
        [NativeFunction("RemoveChild")]
        internal extern static Transform Internal_RemoveChild(Transform transform, uint index);
        [MethodImpl(MethodImplOptions.InternalCall)]
        [NativeFunction("RemoveChildTransform")]
        internal extern static Transform Internal_RemoveChildTransform(Transform transform, Transform child);

    }

}
