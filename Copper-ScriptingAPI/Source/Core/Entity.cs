using System;
using System.Runtime.InteropServices;
using System.Runtime.CompilerServices;

namespace Copper {

    [StructLayout(LayoutKind.Sequential)]
    public class Entity : Base {

        public string name {
            
            [MethodImpl(MethodImplOptions.InternalCall)]
            get;
            [MethodImpl(MethodImplOptions.InternalCall)]
            set;

        }
        
        public Transform transform {

            [MethodImpl(MethodImplOptions.InternalCall)]
            get;

        }

        public bool HasComponent<T>() where T : Component, new() {

            return Internal_HasComponent(this, typeof(T));

        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static bool Internal_HasComponent(Entity entity, Type type);

    }

}
