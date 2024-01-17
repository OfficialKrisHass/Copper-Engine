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

            get { return Internal_GetIsStatic(componentPointer); }
            set { Internal_SetIsStatic(componentPointer, value); }

        }
        public bool gravity {

            get { return Internal_GetGravity(componentPointer); }
            set { Internal_SetGravity(componentPointer, value); }

        }
        public float mass {

            get { return Internal_GetMass(componentPointer); }
            set { Internal_SetMass(componentPointer, value); }

        }

        public Collider collider {

            get {

                if (!HasComponent<Collider>()) return null;

                Collider ret = new Collider();
                Internal_SetComponentEID(typeof(Collider), ret, eID);
                Internal_SetComponentPointer(typeof(Collider), ret, eID);

                return ret;

            }

        }

        public void AddForce(Vector3 force, ForceMode mode) {

            Internal_AddForce(componentPointer, force, (byte) mode);

        }
        public void AddTorque(Vector3 torque, ForceMode mode) {

            Internal_AddTorque(componentPointer, torque, (byte) mode);

        }

        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static bool Internal_GetIsStatic(long ComponentPointer);
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static bool Internal_GetGravity(long ComponentPointer);
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static float Internal_GetMass(long ComponentPointer);

        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static void Internal_SetIsStatic(long ComponentPointer, bool value);
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static void Internal_SetGravity(long ComponentPointer, bool value);
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static void Internal_SetMass(long ComponentPointer, float value);

        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static void Internal_AddForce(long ComponentPointer, Vector3 force, byte mode);
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static void Internal_AddTorque(long ComponentPointer, Vector3 torque, byte mode);

    }

}