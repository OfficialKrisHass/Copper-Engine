using System;

namespace Copper {

    public class Transform : BuiltInComponent {

        protected Transform() {

            this.eID = 4294967295;

        }
        public Transform(uint eID) {

            this.eID = eID;

        }

        public Vector3 position {

            get {

                Vector3 output;
                InternalCalls.GetPosition(eID, out output);

                return output;

            }
            set {

                InternalCalls.SetPosition(eID, value);

            }

        }
        public Quaternion rotation {

            get {

                Quaternion output;
                InternalCalls.GetRotation(eID, out output);

                return output;

            }
            set {

                InternalCalls.SetRotation(eID, value);

            }

        }
        public Vector3 scale {

            get {

                Vector3 output;
                InternalCalls.GetScale(eID, out output);

                return output;

            }
            set {

                InternalCalls.SetScale(eID, value);

            }

        }

        public Vector3 forward {

            get {

                Vector3 output;
                InternalCalls.GetForward(eID, out output);
                return output;

            }
        
        }
        public Vector3 right {

            get {

                Vector3 output;
                InternalCalls.GetRight(eID, out output);
                return output;

            }

        }
        public Vector3 up {

            get {

                Vector3 output;
                InternalCalls.GetUp(eID, out output);
                return output;

            }

        }

    }

}