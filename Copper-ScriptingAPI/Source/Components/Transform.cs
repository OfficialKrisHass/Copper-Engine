using System;
using System.Runtime.CompilerServices;

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
                Internal_GetPosition(eID, out output);

                return output;

            }
            set {

                Internal_SetPosition(eID, value);

            }

        }
        public Quaternion rotation {

            get {

                Quaternion output;
                Internal_GetRotation(eID, out output);

                return output;

            }
            set {

                Internal_SetRotation(eID, value);

            }

        }
        public Vector3 scale {

            get {

                Vector3 output;
                Internal_GetScale(eID, out output);

                return output;

            }
            set {

                Internal_SetScale(eID, value);

            }

        }

        public Vector3 forward {

            get {

                Vector3 output;
                Internal_GetForward(eID, out output);
                return output;

            }
        
        }
        public Vector3 right {

            get {

                Vector3 output;
                Internal_GetRight(eID, out output);
                return output;

            }

        }
        public Vector3 up {

            get {

                Vector3 output;
                Internal_GetUp(eID, out output);
                return output;

            }

        }

        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static void Internal_GetPosition(uint eID, out Vector3 position);
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static void Internal_GetRotation(uint eID, out Quaternion rotation);
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static void Internal_GetScale(uint eID, out Vector3 scale);
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static void Internal_SetPosition(uint eID, Vector3 position);
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static void Internal_SetRotation(uint eID, Quaternion rotation);
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static void Internal_SetScale(uint eID, Vector3 scale);

        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static void Internal_GetForward(uint eID, out Vector3 forward);
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static void Internal_GetRight(uint eID, out Vector3 right);
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static void Internal_GetUp(uint eID, out Vector3 up);

    }

}