using System;
using System.Runtime.CompilerServices;

namespace Copper {

    public class BoxCollider : Collider {

        public Vector3 size {

            get {

                Vector3 ret;
                Internal_GetSize(componentPointer, out ret);

                return ret;

            }
            set { Internal_SetSize(componentPointer, value); }

        }

        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static void Internal_GetSize(long componentPointer, out Vector3 size);
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static void Internal_SetSize(long componentPointer, Vector3 value);

    }

}