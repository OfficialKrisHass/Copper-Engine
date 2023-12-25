using System;
using System.Runtime.CompilerServices;

namespace Copper {

    public class Camera : BuiltInComponent {

        public float fov {

            get { return Internal_GetFOV(eID); }
            set { Internal_SetFOV(eID, value); }

        }
        public float nearPlane {

            get { return Internal_GetNearPlane(eID); }
            set { Internal_SetNearPlane(eID, value); }

        }
        public float farPlane {

            get { return Internal_GetFarPlane(eID); }
            set { Internal_SetFarPlane(eID, value); }

        }

        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static float Internal_GetFOV(uint eID);
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static float Internal_GetNearPlane(uint eID);
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static float Internal_GetFarPlane(uint eID);
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static void Internal_SetFOV(uint eID, float value);
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static void Internal_SetNearPlane(uint eID, float value);
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static void Internal_SetFarPlane(uint eID, float value);

    }

}