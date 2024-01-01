using System.Runtime.CompilerServices;

namespace Copper {

    public class Collider : BuiltInComponent {

        public bool trigger {

            get { return Internal_GetTrigger(componentPointer); }
            set { Internal_SetTrigger(componentPointer, value); }

        }

        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static bool Internal_GetTrigger(long componentPointer);
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static void Internal_SetTrigger(long componentPointer, bool value);

    }

}