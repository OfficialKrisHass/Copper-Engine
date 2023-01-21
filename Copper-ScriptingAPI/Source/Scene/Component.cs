using System;

namespace Copper {

    public class Component {

        protected int objID;

        protected Component() { }
        protected Component(int objID) {

            this.objID = objID;

        }

        public CopperObject obj {

            get {

                return new CopperObject(objID);

            }

        }
        public Transform transform {

            get {
                return new Transform(objID);
            }

        }

        public T AddComponent<T>() where T : Component, new() {

            T ret = new T();
            InternalCalls.AddComponent(objID, typeof(T), ret);

            Editor.Log(ret.objID.ToString());

            return ret;

        }
        public T GetComponent<T>() where T : Component, new() {

            T ret = new T();

            Type type = typeof(T);
            if (type == typeof(Camera) || type == typeof(Transform)) { ret.objID = objID; return ret; }

            if (!InternalCalls.GetComponent(objID, type, ret)) return null;

            Editor.Log(ret.objID.ToString());

            return ret;

        }
        public bool HasComponent<T>() where T : Component, new() {

            return InternalCalls.HasComponent(objID, typeof(T));

        }

    }

}