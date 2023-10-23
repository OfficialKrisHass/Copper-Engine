using System;

namespace Copper {

    public class Component {

        protected uint eID;

        protected Component() {

            eID = Entity.invalidID;
            
        }
        protected Component(uint eID) {

            this.eID = eID;

        }

        public Entity obj {

            get {

                return InternalCalls.GetEntity(eID);

            }

        }
        public Transform transform {

            get {
                return new Transform(eID);
            }

        }

        public T AddComponent<T>() where T : Component, new() {

            T ret = new T();
            InternalCalls.AddComponent(eID, typeof(T), ret);

            Editor.Log(ret.eID.ToString());

            return ret;

        }
        public T GetComponent<T>() where T : Component, new() {

            T ret = new T();

            Type type = typeof(T);
            if (type == typeof(Camera) || type == typeof(Transform) || type == typeof(RigidBody)) {

                if (!HasComponent<T>()) return null;
                ret.eID = eID;

                return ret;

            }

            if (!InternalCalls.GetComponent(eID, type, ret)) return null;

            Editor.Log(ret.eID.ToString());

            return ret;

        }
        public bool HasComponent<T>() where T : Component, new() {

            return InternalCalls.HasComponent(eID, typeof(T));

        }

    }

}