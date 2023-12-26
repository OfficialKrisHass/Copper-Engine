using System;
using System.Runtime.CompilerServices;

namespace Copper {

    public class Component {

        protected uint eID;

        protected Component() {

            eID = 4294967295;
            
        }
        protected Component(uint eID) {

            this.eID = eID;

        }

        public Entity entity {

            get {

                return Entity.Internal_GetEntity(eID);

            }

        }
        public Transform transform {

            get {

                return new Transform(eID);

            }

        }

        public T AddComponent<T>() where T : Component, new() {

            T ret = new T();
            Internal_AddComponent(eID, typeof(T), ret);

            return ret;

        }
        public T GetComponent<T>() where T : Component, new() {

            T ret = new T();

            Type type = typeof(T);
            if (type.BaseType == typeof(BuiltInComponent)) {

                if (!HasComponent<T>()) return null;
                ret.eID = eID;

                return ret;

            }

            if (!Internal_GetComponent(eID, type, ret)) return null;

            Editor.Log(ret.eID.ToString());

            return ret;

        }
        public bool HasComponent<T>() where T : Component, new() {

            return Internal_HasComponent(eID, typeof(T));

        }

        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static void Internal_AddComponent(uint eID, Type type, Component component);
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static bool Internal_GetComponent(uint eID, Type type, Component component);
        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static bool Internal_HasComponent(uint eID, Type type);

        [MethodImpl(MethodImplOptions.InternalCall)] internal extern static void Internal_SetComponentEID(Type type, Component component, uint eID);

    }

}