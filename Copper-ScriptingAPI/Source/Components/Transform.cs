using System;
using System.Runtime.CompilerServices;

namespace Copper {

    public class Transform : BuiltInComponent {

        protected Transform() {

            this.eID = 4294967295;
            componentPointer = 0;

        }
        public Transform(uint eID) {

            this.eID = eID;
            Internal_SetComponentPointer(typeof(Transform), this, eID);

        }

        public Vector3 position {

            get {

                Vector3 output;
                Internal_GetPosition(componentPointer, out output);

                return output;

            }
            set {

                Internal_SetPosition(componentPointer, value);

            }

        }
        public Quaternion rotation {

            get {

                Quaternion output;
                Internal_GetRotation(componentPointer, out output);

                return output;

            }
            set {

                Internal_SetRotation(componentPointer, value);

            }

        }
        public Vector3 scale {

            get {

                Vector3 output;
                Internal_GetScale(componentPointer, out output);

                return output;

            }
            set {

                Internal_SetScale(componentPointer, value);

            }

        }

        public Vector3 globalPosition {

            get {

                Vector3 output;
                Internal_GetGlobalPosition(componentPointer, out output);

                return output;

            }
            set {

                Internal_SetGlobalPosition(componentPointer, value);

            }

        }
        public Quaternion globalRotation {

            get {

                Quaternion output;
                Internal_GetGlobalRotation(componentPointer, out output);

                return output;

            }
            set {

                Internal_SetGlobalRotation(componentPointer, value);

            }

        }
        public Vector3 globalScale {

            get {

                Vector3 output;
                Internal_GetGlobalScale(componentPointer, out output);

                return output;

            }
            set {

                Internal_SetGlobalScale(componentPointer, value);

            }

        }

        public Vector3 forward {

            get {

                Vector3 output;
                Internal_GetForward(componentPointer, out output);
                return output;

            }
        
        }
        public Vector3 right {

            get {

                Vector3 output;
                Internal_GetRight(componentPointer, out output);
                return output;

            }

        }
        public Vector3 up {

            get {

                Vector3 output;
                Internal_GetUp(componentPointer, out output);
                return output;

            }

        }

        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static void Internal_GetPosition(long componentPointer, out Vector3 position);
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static void Internal_GetRotation(long componentPointer, out Quaternion rotation);
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static void Internal_GetScale(long componentPointer, out Vector3 scale);
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static void Internal_GetGlobalPosition(long componentPointer, out Vector3 position);
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static void Internal_GetGlobalRotation(long componentPointer, out Quaternion rotation);
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static void Internal_GetGlobalScale(long componentPointer, out Vector3 scale);

        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static void Internal_GetForward(long componentPointer, out Vector3 forward);
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static void Internal_GetRight(long componentPointer, out Vector3 right);
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static void Internal_GetUp(long componentPointer, out Vector3 up);

        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static void Internal_SetPosition(long componentPointer, Vector3 position);
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static void Internal_SetRotation(long componentPointer, Quaternion rotation);
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static void Internal_SetScale(long componentPointer, Vector3 scale);
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static void Internal_SetGlobalPosition(long componentPointer, Vector3 position);
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static void Internal_SetGlobalRotation(long componentPointer, Quaternion rotation);
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static void Internal_SetGlobalScale(long componentPointer, Vector3 scale);

    }

}