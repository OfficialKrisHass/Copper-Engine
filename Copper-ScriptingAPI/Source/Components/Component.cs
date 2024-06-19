using System;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;

namespace Copper {

    [StructLayout(LayoutKind.Sequential)]
    [NativeClass]
    public class Component : Base {

        public Entity entity {

            [MethodImpl(MethodImplOptions.InternalCall)]
            get;

        }
        public Transform transform {

            [MethodImpl(MethodImplOptions.InternalCall)]
            get;

        }

        public T AddComponent<T>() where T : Component, new() { return entity.AddComponent<T>(); }
        public T GetComponent<T>() where T : Component, new() { return entity.GetComponent<T>(); }
        public bool HasComponent<T>() where T : Component, new() { return entity.HasComponent<T>(); }
        public void RemoveComponent<T>() where T : Component, new() { entity.RemoveComponent<T>(); }

    }

}
