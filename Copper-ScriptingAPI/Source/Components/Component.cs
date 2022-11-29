using System;

namespace Copper {

    public class Component {

        public int objID;
        public Transform transform;

        protected Component() { }

        public Component(int id) {

            objID = id;
            transform = new Transform(id);

        }

        public Object obj {

            get {
                InternalFunctions.GetObject(objID, out Object result);
                return result;
            }

        }

        public bool HasComponent<T>() where T : Component, new() {

            Type cType = typeof(T);
            return InternalFunctions.HasComponent(objID, cType);

        }
        public T GetComponent<T>() where T : Component, new() {

            T ret = new T() { objID = objID };

            Type cType = typeof(T);
            InternalFunctions.GetComponent(objID, cType, ret);

            return ret;

        }

    }

}