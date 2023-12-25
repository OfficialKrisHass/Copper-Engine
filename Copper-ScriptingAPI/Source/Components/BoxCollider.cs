using System;
using System.Runtime.CompilerServices;

namespace Copper {

    public class BoxCollider : BuiltInComponent {

        public bool trigger {

            get { return Internal_GetTrigger(eID); }
            set { Internal_SetTrigger(eID, value); }

        }

        public Vector3 center {

            get {

                Vector3 ret;
                Internal_GetCenter(eID, out ret);

                return ret;
                
            }
            set { Internal_SetCenter(eID, value); }

        }
        public Vector3 size {

            get {

                Vector3 ret;
                Internal_GetSize(eID, out ret);

                return ret;
                
            }
            set { Internal_SetSize(eID, value); }

        }

        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static bool Internal_GetTrigger(uint eID);
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static void Internal_GetCenter(uint eID, out Vector3 center);
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static void Internal_GetSize(uint eID, out Vector3 size);

        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static void Internal_SetTrigger(uint eID, bool value);
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static void Internal_SetCenter(uint eID, Vector3 value);
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static void Internal_SetSize(uint eID, Vector3 value);

    }

}