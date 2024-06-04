using System;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;

namespace Copper {

    [StructLayout(LayoutKind.Sequential)]
    public class Component : Base {

        public Entity entity {

            [MethodImpl(MethodImplOptions.InternalCall)]
            get;

        }
        public Transform transform {

            [MethodImpl(MethodImplOptions.InternalCall)]
            get;

        }

        public bool HasComponent<T>() where T : Component, new() { return entity.HasComponent<T>(); }

    }

}
