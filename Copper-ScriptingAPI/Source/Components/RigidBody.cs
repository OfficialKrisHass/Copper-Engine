using System;

namespace Copper {

    public enum ForceMode {

        Force,
        Impulse,
        VelocityChange,
        Acceleration

    }

    public class RigidBody : BuiltInComponent {

        public bool isStatic {

            get { return InternalCalls.RigidBodyGetIsStatic(eID); }
            set { InternalCalls.RigidBodySetIsStatic(eID, value); }

        }
        public bool gravity {

            get { return InternalCalls.RigidBodyGetGravity(eID); }
            set { InternalCalls.RigidBodySetGravity(eID, value); }

        }

        public float mass {

            get { return InternalCalls.RigidBodyGetMass(eID); }
            set { InternalCalls.RigidBodySetMass(eID, value); }

        }

        public void AddForce(Vector3 force, ForceMode mode) {

            InternalCalls.RigidBodyAddForce(eID, force, (byte) mode);

        }
        public void AddTorque(Vector3 torque, ForceMode mode) {

            InternalCalls.RigidBodyAddTorque(eID, torque, (byte) mode);

        }

    }

}