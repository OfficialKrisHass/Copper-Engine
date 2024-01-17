using System;
using System.Runtime.CompilerServices;

namespace Copper {

    public class Entity {

        protected uint id;

        protected Entity() {

            id = 4294967295;

        }
        protected Entity(uint id) {

            this.id = id;

        }

        public string name {

            get {
                
                return Internal_GetEntityName(id);

            }
            set {

                Internal_SetEntityName(id, value);

            }

        }

        public Transform transform {

            get {

                return new Transform(id);

            }

        }

        public T AddComponent<T>() where T : Component, new() {

            T ret = new T();
            Component.Internal_AddComponent(id, typeof(T), ret);

            return ret;

        }
        public T GetComponent<T>() where T : Component, new() {

            T ret = new T();

            Type type = typeof(T);
            if (type == typeof(Camera) || type == typeof(Transform)) {

                if (!HasComponent<T>()) return null;

                Component.Internal_SetComponentEID(type, ret, id); //We have to set the Component ID from C++ since
                return ret;                                   //Component.eID is private (no I Can't make it public)
            
            }

            if (!Component.Internal_GetComponent(id, type, ret)) return null;

            return ret;

        }
        public bool HasComponent<T>() where T : Component, new() {

            return Component.Internal_HasComponent(id, typeof(T));

        }

        public static implicit operator bool(Entity entity) { return Internal_IsEntityValid(entity.id); }

        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static string Internal_GetEntityName(uint eID);
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static void Internal_SetEntityName(uint eID, string name);
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static bool Internal_IsEntityValid(uint eID);
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static Entity Internal_GetEntity(uint eID);

    }

}