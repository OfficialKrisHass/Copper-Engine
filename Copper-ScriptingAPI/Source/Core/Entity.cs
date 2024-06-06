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

        public T AddComponent<T>() where T : Component, new() { return (T) Internal_AddComponent(this, typeof(T)); }
        public T GetComponent<T>() where T : Component, new() { return (T) Internal_GetComponent(this, typeof(T)); }
        public bool HasComponent<T>() where T : Component, new() { return Internal_HasComponent(this, typeof(T)); }
        public void RemoveComponent<T>() where T : Component, new() { Internal_RemoveComponent(this, typeof(T)); }

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static object Internal_AddComponent(Entity entity, Type type);
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static object Internal_GetComponent(Entity entity, Type type);
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static bool Internal_HasComponent(Entity entity, Type type);
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void Internal_RemoveComponent(Entity entity, Type type);

    }

}
