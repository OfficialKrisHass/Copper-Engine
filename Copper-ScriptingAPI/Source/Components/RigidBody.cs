using System;
using System.Runtime.InteropServices;
using System.Runtime.CompilerServices;

namespace Copper {

    [StructLayout(LayoutKind.Sequential)]
    public class RigidBody : Component {

        public static int ComponentID() { return 5; }

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

        public extern bool isStatic {

            [MethodImpl(MethodImplOptions.InternalCall)]
            get;
            [MethodImpl(MethodImplOptions.InternalCall)]
            set;

        }
        public extern bool gravity {

            [MethodImpl(MethodImplOptions.InternalCall)]
            get;
            [MethodImpl(MethodImplOptions.InternalCall)]
            set;

        }

        public extern float mass {

            [MethodImpl(MethodImplOptions.InternalCall)]
            get;
            [MethodImpl(MethodImplOptions.InternalCall)]
            set;

        }

        public extern byte lockMask {

            [MethodImpl(MethodImplOptions.InternalCall)]
            get;
            [MethodImpl(MethodImplOptions.InternalCall)]
            set;

        }

        public void AddForce(Vector3 force, ForceMode mode = ForceMode.Force) { Internal_AddForce(this, ref force, mode); }
        public void AddTorque(Vector3 torque, ForceMode mode = ForceMode.Force) { Internal_AddTorque(this, ref torque, mode); }

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void Internal_AddForce(RigidBody rb, ref Vector3 force, ForceMode mode);
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void Internal_AddTorque(RigidBody rb, ref Vector3 torque, ForceMode mode);

    }

}