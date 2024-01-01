using System;
using System.Runtime.CompilerServices;

namespace Copper {

    public class SphereCollider : Collider {

        public float radius {

            get { return Internal_GetRadius(componentPointer); }
            set { Internal_SetRadius(componentPointer, value); }

        }

        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static float Internal_GetRadius(long componentPointer);
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static void Internal_SetRadius(long componentPointer, float value);

    }

}