using System;

namespace Copper {

    public class BoxCollider : Component {

        public bool trigger {

            get { return InternalCalls.BoxColliderGetTrigger(eID); }
            set { InternalCalls.BoxColliderSetTrigger(eID, value); }

        }

        public Vector3 center {

            get {

                Vector3 ret;
                InternalCalls.BoxColliderGetCenter(eID, out ret);

                return ret;
                
            }
            set { InternalCalls.BoxColliderSetCenter(eID, value); }

        }
        public Vector3 size {

            get {

                Vector3 ret;
                InternalCalls.BoxColliderGetSize(eID, out ret);

                return ret;
                
            }
            set { InternalCalls.BoxColliderSetSize(eID, value); }

        }

    }

}