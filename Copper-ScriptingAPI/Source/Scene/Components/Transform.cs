using System;

namespace Copper {

    public class Transform : Component {

        public Transform(int objID) {

            base.objID = objID;

        }

        public Vector3 position {

            get {

                Vector3 output;
                InternalCalls.GetPosition(objID, out output);

                return output;

            }
            set {

                InternalCalls.SetPosition(objID, value);

            }

        }
        public Vector3 rotation {

            get {

                Vector3 output;
                InternalCalls.GetRotation(objID, out output);

                return output;

            }
            set {

                InternalCalls.SetRotation(objID, value);

            }

        }
        public Vector3 scale {

            get {

                Vector3 output;
                InternalCalls.GetScale(objID, out output);

                return output;

            }
            set {

                InternalCalls.SetScale(objID, value);

            }

        }

    }

}