using System;

namespace Copper {

    public class Entity {

        protected uint id;
        const uint invalidID = 4294967295;

        protected Entity() {

            id = invalidID;

        }
        protected Entity(uint id) {

            this.id = id;

        }

        public string name {

            get {

                return InternalCalls.GetEntityName(id);

            }
            set {

                InternalCalls.SetEntityName(id, value);

            }

        }

        public Transform transform {

            get {

                return new Transform(id);

            }

        }

        public T AddComponent<T>() where T : Component, new() {

            T ret = new T();
            InternalCalls.AddComponent(id, typeof(T), ret);

            return ret;

        }
        public T GetComponent<T>() where T : Component, new() {

            T ret = new T();

            Type type = typeof(T);
            if (type == typeof(Camera) || type == typeof(Transform)) {

                if (!HasComponent<T>()) return null;

                InternalCalls.SetComponentEID(type, ret, id); //We have to set the Component ID from C++ since
                return ret;                                   //Component.eID is private (no I Can't make it public)
            
            }

            if (!InternalCalls.GetComponent(id, type, ret)) return null;

            return ret;

        }
        public bool HasComponent<T>() where T : Component, new() {

            return InternalCalls.HasComponent(id, typeof(T));

        }

    }

}