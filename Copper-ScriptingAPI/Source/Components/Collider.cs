using System.Runtime.CompilerServices;

namespace Copper {

    public class Collider : BuiltInComponent {

        public bool trigger {

            get { return Internal_GetTrigger(componentPointer); }
            set { Internal_SetTrigger(componentPointer, value); }

        }

        public Vector3 center {

            get {

                Vector3 ret;
                Internal_GetCenter(componentPointer, out ret);

                return ret;

            }
            set { Internal_SetCenter(componentPointer, value); }

        }

        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static bool Internal_GetTrigger(long componentPointer);
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static void Internal_GetCenter(long componentPointer, out Vector3 center);

        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static void Internal_SetTrigger(long componentPointer, bool value);
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static void Internal_SetCenter(long componentPointer, Vector3 value);

    }

}