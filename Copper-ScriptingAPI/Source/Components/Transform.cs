using System;

namespace Copper {

    public class Transform {

        public int objID;

        public Transform(int id) {

            objID = id;

        }

        public Object obj {

            get {
                InternalFunctions.GetObject(objID, out Object result);
                return result;
            }

        }

        public Vector3 position {

            get {

                InternalFunctions.TransformGetPosition(objID, out Vector3 result);
                return result;

            }
            set {

                InternalFunctions.TransformSetPosition(objID, ref value);

            }

        }
        public Vector3 rotation {

            get {

                InternalFunctions.TransformGetRotation(objID, out Vector3 result);
                return result;

            }
            set {

                InternalFunctions.TransformSetRotation(objID, ref value);

            }

        }
        public Vector3 scale {

            get {

                InternalFunctions.TransformGetScale(objID, out Vector3 result);
                return result;

            }
            set {

                InternalFunctions.TransformSetScale(objID, ref value);

            }

        }

    }

}