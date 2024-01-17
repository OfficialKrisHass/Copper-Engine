using System;
using System.Runtime.CompilerServices;

namespace Copper {

    public class Camera : BuiltInComponent {

        public float fov {

            get { return Internal_GetFOV(componentPointer); }
            set { Internal_SetFOV(componentPointer, value); }

        }
        public float nearPlane {

            get { return Internal_GetNearPlane(componentPointer); }
            set { Internal_SetNearPlane(componentPointer, value); }

        }
        public float farPlane {

            get { return Internal_GetFarPlane(componentPointer); }
            set { Internal_SetFarPlane(componentPointer, value); }

        }

        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static float Internal_GetFOV(long componentPointer);
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static float Internal_GetNearPlane(long componentPointer);
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static float Internal_GetFarPlane(long componentPointer);
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static void Internal_SetFOV(long componentPointer, float value);
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static void Internal_SetNearPlane(long componentPointer, float value);
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static void Internal_SetFarPlane(long componentPointer, float value);

    }

}