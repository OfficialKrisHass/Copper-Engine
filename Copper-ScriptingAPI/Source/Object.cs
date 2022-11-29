using System;

namespace Copper {

    public class Object {

        private readonly int ID;
        public string name {

            get {
                InternalFunctions.GetObjectName(ID, out string result);
                return result;
            }
            set {
                InternalFunctions.SetObjectName(ID, ref value);
            }

        }

        public bool HasComponent<T>() where T : Component, new() {

            Type cType = typeof(T);
            return InternalFunctions.HasComponent(ID, cType);

        }
        public T GetComponent<T>() where T : Component, new() {

            T ret = new T() { objID = ID };

            Type cType = typeof(T);
            InternalFunctions.GetComponent(ID, cType, ret);

            return ret;
        }

    }

}