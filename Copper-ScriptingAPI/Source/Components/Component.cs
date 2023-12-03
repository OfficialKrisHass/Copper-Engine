using System;

namespace Copper {

    public class Component {

        protected uint eID;

        protected Component() {

            eID = 4294967295;
            
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

                return new Transform();

            }

        }

        public T AddComponent<T>() where T : Component, new() {

            T ret = new T();
            InternalCalls.AddComponent(eID, typeof(T), ret);

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

            if (!InternalCalls.GetComponent(eID, type, ret)) return null;

            Editor.Log(ret.eID.ToString());

            return ret;

        }
        public bool HasComponent<T>() where T : Component, new() {

            return InternalCalls.HasComponent(eID, typeof(T));

        }

    }

}