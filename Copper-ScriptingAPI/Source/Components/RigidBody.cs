using System;

namespace Copper {

    public class RigidBody : Component {

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

    }

}