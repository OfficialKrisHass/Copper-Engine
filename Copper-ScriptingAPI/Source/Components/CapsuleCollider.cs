using System;
using System.Runtime.CompilerServices;

namespace Copper {

    public class CapsuleCollider : Collider {

        public float radius {

            get { return Internal_GetRadius(componentPointer); }
            set { Internal_SetRadius(componentPointer, value); }

        }
        public float height {

            get { return Internal_GetHeight(componentPointer); }
            set { Internal_SetHeight(componentPointer, value); }

        }

        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static float Internal_GetRadius(long componentPointer);
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static float Internal_GetHeight(long componentPointer);

        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static void Internal_SetRadius(long componentPointer, float value);
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static void Internal_SetHeight(long componentPointer, float value);

    }

}