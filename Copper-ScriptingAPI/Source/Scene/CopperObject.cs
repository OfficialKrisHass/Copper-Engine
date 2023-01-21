using System;

namespace Copper {

    public class CopperObject {

        protected int objID;

        public CopperObject() {

            objID = -1;
            Editor.Log("Default Constructor!");

        }
        public CopperObject(int objID) {

            this.objID = objID;

        }

        public string name {

            get {

                return InternalCalls.GetObjectName(objID);

            }
            set {

                InternalCalls.SetObjectName(objID, value);

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

            return ret;

        }
        public T GetComponent<T>() where T : Component, new() {

            T ret = new T();

            Type type = typeof(T);
            if (type == typeof(Camera) || type == typeof(Transform)) {
                
                InternalCalls.SetComponentObjID(type, ret, objID); //We have to set the Component ID from C++ since
                return ret;                                        //Component.objID is private (no I Can't change it)
            
            }

            if (!InternalCalls.GetComponent(objID, type, ret)) return null;

            return ret;

        }
        public bool HasComponent<T>() where T : Component, new() {

            return InternalCalls.HasComponent(objID, typeof(T));

        }

    }

}