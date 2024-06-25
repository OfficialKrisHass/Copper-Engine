using System;
using System.Runtime.InteropServices;
using System.Runtime.CompilerServices;

namespace Copper {

    [StructLayout(LayoutKind.Sequential)]
    [NativeClass("Engine/Scripting/InternalCalls/Components/RigidBody.cs.h")]
    public class RigidBody : Component {

        private static uint ComponentID() { return 5; } 

        public enum ForceMode : byte {

            Force = 0,
            Impulse = 1,
            VelocityChange = 2,
            Acceleration = 3,

        };

        public enum LockMask : byte {

            PosX = 1, PosY = 2, PosZ = 4,
            RotX = 8, RotY = 16, RotZ = 32,

        };

        public extern float mass {

            [MethodImpl(MethodImplOptions.InternalCall)]
            [NativeFunction("GetMass")]
            get;
            [MethodImpl(MethodImplOptions.InternalCall)]
            [NativeFunction("SetMass")]
            set;

        }

        public extern bool isStatic {

            [MethodImpl(MethodImplOptions.InternalCall)]
            [NativeFunction("GetIsStatic")]
            get;
            [MethodImpl(MethodImplOptions.InternalCall)]
            [NativeFunction("SetIsStatic")]
            set;

        }
        public extern bool gravity {

            [MethodImpl(MethodImplOptions.InternalCall)]
            [NativeFunction("GetGravity")]
            get;
            [MethodImpl(MethodImplOptions.InternalCall)]
            [NativeFunction("SetGravity")]
            set;

        }
        public extern byte lockMask {

            [MethodImpl(MethodImplOptions.InternalCall)]
            [NativeFunction("GetLockMask")]
            get;
            [MethodImpl(MethodImplOptions.InternalCall)]
            [NativeFunction("SetLockMask")]
            set;

        }

        public void AddForce(Vector3 force, ForceMode mode = ForceMode.Force) { Internal_AddForce(this, ref force, mode); }
        public void AddTorque(Vector3 torque, ForceMode mode = ForceMode.Force) { Internal_AddTorque(this, ref torque, mode); }

        [MethodImpl(MethodImplOptions.InternalCall)]
        [NativeFunction("AddForce")]
        internal extern static void Internal_AddForce(RigidBody rb, ref Vector3 force, ForceMode mode);
        [MethodImpl(MethodImplOptions.InternalCall)]
        [NativeFunction("AddTorque")]
        internal extern static void Internal_AddTorque(RigidBody rb, ref Vector3 torque, ForceMode mode);

    }

}
