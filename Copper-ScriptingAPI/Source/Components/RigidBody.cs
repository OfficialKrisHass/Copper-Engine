using System;
using System.Runtime.CompilerServices;

namespace Copper {

    public enum ForceMode {

        Force,
        Impulse,
        VelocityChange,
        Acceleration

    }

    public class RigidBody : BuiltInComponent {

        public bool isStatic {

            get { return Internal_GetIsStatic(eID); }
            set { Internal_SetIsStatic(eID, value); }

        }
        public bool gravity {

            get { return Internal_GetGravity(eID); }
            set { Internal_SetGravity(eID, value); }

        }

        public float mass {

            get { return Internal_GetMass(eID); }
            set { Internal_SetMass(eID, value); }

        }

        public void AddForce(Vector3 force, ForceMode mode) {

            Internal_AddForce(eID, force, (byte) mode);

        }
        public void AddTorque(Vector3 torque, ForceMode mode) {

            Internal_AddTorque(eID, torque, (byte) mode);

        }

        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static bool Internal_GetIsStatic(uint eID);
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static bool Internal_GetGravity(uint eID);
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static float Internal_GetMass(uint eID);

        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static void Internal_SetIsStatic(uint eID, bool value);
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static void Internal_SetGravity(uint eID, bool value);
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static void Internal_SetMass(uint eID, float value);

        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static void Internal_AddForce(uint eID, Vector3 force, byte mode);
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static void Internal_AddTorque(uint eID, Vector3 torque, byte mode);

    }

}