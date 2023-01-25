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

        public Vector3 forward {

            get {

                Vector3 output;
                InternalCalls.GetForward(objID, out output);
                return output;

            }
        
        }
        public Vector3 back {

            get {

                Vector3 output;
                InternalCalls.GetBack(objID, out output);
                return output;

            }

        }
        public Vector3 right {

            get {

                Vector3 output;
                InternalCalls.GetRight(objID, out output);
                return output;

            }

        }
        public Vector3 left {

            get {

                Vector3 output;
                InternalCalls.GetLeft(objID, out output);
                return output;

            }

        }
        public Vector3 up {

            get {

                Vector3 output;
                InternalCalls.GetUp(objID, out output);
                return output;

            }

        }
        public Vector3 down {

            get {

                Vector3 output;
                InternalCalls.GetDown(objID, out output);
                return output;

            }

        }

    }

}